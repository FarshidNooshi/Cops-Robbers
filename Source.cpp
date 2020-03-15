// In The Name Of GOD
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<unistd.h>

#define MAXN 128

const int px[] = { 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0 };
const int py[] = { 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1 };
const int pz[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

struct Person {
	int x, y, z;
} Thief[MAXN], CopyT[MAXN];

struct Station {
	int sz, NumSaw;
	struct Person police[MAXN];
	int sawed[MAXN];
} Department[MAXN];

int n, m, k, tme, LG, NumS, NumT, table[MAXN][MAXN][MAXN];

int abs(int x);
void UpdCops();
void CopsMove();
void PRINT(int);
void CreateCops();
void ThievesMove();
void CatchThieves();
void CreateThieves();
int get_nearest(struct Person rex, int rexy);
int valid(int x, int y, int z);
int GetNearest(struct Person a, struct Person b);

int main() {
	srand(time(NULL));
	printf("Be tartib: tool, arz, ertefa, tedade kalantari ha, tedade dozd ha, ro vared konid\n");
	scanf("%d%d%d%d%d", &n, &m, &k, &NumS, &NumT);
	int sm = 0;
	LG = NumS ? (int)log10(NumS) + 2 : 1;
	printf("tedade police baraye har kalantari ro vared konid\n");
	for (int i = 0; i < NumS; i++) {
		printf("Tedade police morede nazar baraye kalantari %d ro vared konid\n", i + 1);
		scanf("%d", &Department[i].sz), sm += Department[i].sz;
	}
	if (sm + NumT > n* m* k) {
		printf("Invalid Input\n");
		return 0;
	}
	CreateThieves();
	CreateCops();
	UpdCops();
	while (NumT > 0) {
		tme++;
		for (int i = 0; i < k; i++)
			PRINT(i);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				for (int l = 0; l < k; l++)
					table[i][j][l] = 0;
		for (int i = 0; i < NumT; i++)
			CopyT[i] = Thief[i];
		ThievesMove();
		CopsMove();
		CatchThieves();
		UpdCops();
		sleep(1);
		system("cls");
	}
	printf("Done In %d Turns\n", tme);
	return 0;
}

void PRINT(int ZEUS) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (table[i][j][ZEUS] == 0)
				for (int w = 0; w < LG; w++)
					printf("*");
			else if (table[i][j][ZEUS] > 0) {
				printf("D%d", table[i][j][ZEUS]);
				int cnt = 1 + (log10(table[i][j][ZEUS]) + 1);
				for (int w = cnt; w < LG; w++)
					printf(" ");
			}
			else {
				printf("T");
				for (int w = 1; w < LG; w++)
					printf(" ");
			}
			printf(" ");
		}
		printf("\n");
	}
	printf("FLOOR NUMBER %d\n", ZEUS + 1);
}

void CatchThieves() {
	for (int i = 0, Catch = 1; i < NumT; i++, Catch = 1)
		for (int j = 0; j < NumS && Catch; j++)
			for (int l = 0; l < Department[j].sz && Catch; l++)
				if (Department[j].police[l].x == Thief[i].x && Department[j].police[l].y == Thief[i].y
					&& Department[j].police[l].z == Thief[i].z) {
					Thief[i--] = Thief[--NumT];
					Catch = 0;
				}
}

int GetNearest(struct Person a, struct Person b) {
	int dist = 1000, ret = -1;
	for (int tmp = 0; tmp < 27; tmp++) {
		a.x += px[tmp], a.y += py[tmp], a.z += pz[tmp];
		if (!valid(a.x, a.y, a.z) || table[a.x][a.y][a.z] > 0) {
			a.x -= px[tmp], a.y -= py[tmp], a.z -= pz[tmp];
			continue;
		}
		int dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
		if ((dx * dx + dy * dy + dz * dz) < dist)
			dist = dx * dx + dy * dy + dz * dz, ret = tmp;
		a.x -= px[tmp], a.y -= py[tmp], a.z -= pz[tmp];
	}
	return ret;
}

void CopsMove() {
	for (int i = 0; i < NumS; i++)
		if (Department[i].NumSaw)
			for (int j = 0; j < Department[i].sz; j++) {
				int t = GetNearest(Department[i].police[j], CopyT[Department[i].sawed[0]]);
				Department[i].police[j].x += px[t], Department[i].police[j].y += py[t], Department[i].police[j].z += pz[t];
				table[Department[i].police[j].x][Department[i].police[j].y][Department[i].police[j].z] = i + 1;
			}
		else
			for (int j = 0; j < Department[i].sz; j++) {
				int tmp = rand() % 27;
				while (!valid(Department[i].police[j].x + px[tmp], Department[i].police[j].y + py[tmp], Department[i].police[j].z + pz[tmp])
					|| table[Department[i].police[j].x + px[tmp]][Department[i].police[j].y + py[tmp]][Department[i].police[j].z + pz[tmp]] > 0)
					tmp = rand() % 27;
				Department[i].police[j].x += px[tmp], Department[i].police[j].y += py[tmp], Department[i].police[j].z += pz[tmp];
				table[Department[i].police[j].x][Department[i].police[j].y][Department[i].police[j].z] = i + 1;
			}
}

void ThievesMove() {
	for (int i = 0; i < NumT; i++) {
		int dist = -1, t = -1;
		for (int tmp = 0; tmp < 27; tmp++) {
			Thief[i].x += px[tmp], Thief[i].y += py[tmp], Thief[i].z += pz[tmp];
			if (!valid(Thief[i].x, Thief[i].y, Thief[i].z) || table[Thief[i].x][Thief[i].y][Thief[i].z]) {
				Thief[i].x -= px[tmp], Thief[i].y -= py[tmp], Thief[i].z -= pz[tmp];
				continue;
			}
			int v = get_nearest(Thief[i], tmp);
			if (dist < v)
				dist = v, t = tmp;
			Thief[i].x -= px[tmp], Thief[i].y -= py[tmp], Thief[i].z -= pz[tmp];
		}
		if (dist == 1000) {
			int tmp = rand() % 27;
			while (!valid(Thief[i].x + px[tmp], Thief[i].y + py[tmp], Thief[i].z + pz[tmp]) || table[Thief[i].x + px[tmp]][Thief[i].y + py[tmp]][Thief[i].z + pz[tmp]])
				tmp = rand() % 27;
			t = tmp;
		}
		Thief[i].x += px[t], Thief[i].y += py[t], Thief[i].z += pz[t];
		table[Thief[i].x][Thief[i].y][Thief[i].z] = -i - 1;
	}
}

int get_nearest(struct Person rex, int rexy) {
	int ret = 1000;
	rex.x -= px[rexy], rex.y -= py[rexy], rex.z -= pz[rexy];
	for (int i = 0; i < NumS; i++)
		for (int j = 0; j < Department[i].sz; j++)
			if (abs(Department[i].police[j].x - rex.x) < 3 && abs(Department[i].police[j].y - rex.y) < 3
				&& abs(Department[i].police[j].z - rex.z) < 3) {
				int dx = Department[i].police[j].x - rex.x - px[rexy], dy = Department[i].police[j].y - rex.y - py[rexy], dz = Department[i].police[j].z - rex.z - pz[rexy];
				if ((dx * dx + dy * dy + dz * dz) < ret)
					ret = dx * dx + dy * dy + dz * dz;
			}
	return ret;
}

void modify(int tp, int i, int j, int x, int y, int z) {
	if (tp) {
		Department[i].police[j].x = x;
		Department[i].police[j].y = y;
		Department[i].police[j].z = z;
		table[x][y][z] = i + 1;
	}
	else {
		Thief[j].x = x;
		Thief[j].y = y;
		Thief[j].z = z;
		table[x][y][z] = -j - 1;
	}
}

int abs(int x) {
	return x < 0 ? -x : x;
}

void CreateCops() {
	for (int i = 0; i < NumS; i++)
		for (int j = 0; j < Department[i].sz; j++) {
			int x = rand() % n, y = rand() % m, z = rand() % k;
			while (table[x][y][z])
				x = rand() % n, y = rand() % m, z = rand() % k;
			modify(1, i, j, x, y, z);
		}
}

void CreateThieves() {
	for (int i = 0; i < NumT; i++) {
		int x = rand() % n, y = rand() % m, z = rand() % k;
		while (table[x][y][z])
			x = rand() % n, y = rand() % m, z = rand() % k;
		modify(0, 0, i, x, y, z);
	}
}

void UpdCops() {
	for (int i = 0; i < NumS; i++) {
		Department[i].NumSaw = 0;
		for (int j = 0; j < NumT; j++)
			for (int l = 0; l < Department[i].sz; l++)
				if (abs(Thief[j].x - Department[i].police[l].x) < 3 && abs(Thief[j].y - Department[i].police[l].y) < 3
					&& abs(Thief[j].z - Department[i].police[l].z) < 3) {
					Department[i].sawed[Department[i].NumSaw++] = j;
					break;
				}
	}
}

int valid(int x, int y, int z) {
	return x >= 0 && x < n && y >= 0 && y < m && z >= 0 && z < k;
}