#define BOARD_SIZE_2011144024 19
#define BOARD_MID_2011144024 9
#define BLACK -1
#define WHITE 1

typedef struct GAME_BOARD_2011144024 {
	int color;
	int xMax;
	int yMax;
	int uMax;
	int dMax;
	int point;
} GAME_BOARD_2011144024;

GAME_BOARD_2011144024 game_board_2011144024[BOARD_SIZE_2011144024*BOARD_SIZE_2011144024] = { 0 };

char mode_2011144024 = '\n';		// A, B, C ,D
int last_stone_x_2011144024 = -99;
int last_stone_y_2011144024 = -99;
int player_black_turn_2011144024 = 0;
int player_white_turn_2011144024 = 0;

void reset_point_2011144024() {
	for (int i = 0; i < BOARD_SIZE_2011144024*BOARD_SIZE_2011144024; i++) {
		game_board_2011144024[i].point = 0;
	}
}

void took_stone_2011144024(int inner_x, int inner_y, int type) {
	game_board_2011144024[BOARD_SIZE_2011144024*inner_y + inner_x].color = type;
}

void catch_mode_2011144024(int inner_x, int inner_y) {		// East West South North
	if (inner_x < BOARD_MID_2011144024 && inner_y < BOARD_MID_2011144024) mode_2011144024 = 'A';
	else if (inner_x > BOARD_MID_2011144024 && inner_y < BOARD_MID_2011144024) mode_2011144024 = 'B';
	else if (inner_x > BOARD_MID_2011144024 && inner_y > BOARD_MID_2011144024) mode_2011144024 = 'C';
	else if (inner_x < BOARD_MID_2011144024 && inner_y > BOARD_MID_2011144024) mode_2011144024 = 'D';
	else if (inner_y == BOARD_MID_2011144024 && inner_x > BOARD_MID_2011144024) mode_2011144024 = 'E';
	else if (inner_y == BOARD_MID_2011144024 && inner_x < BOARD_MID_2011144024) mode_2011144024 = 'W';
	else if (inner_y > BOARD_MID_2011144024 && inner_x == BOARD_MID_2011144024) mode_2011144024 = 'S';
	else if (inner_y < BOARD_MID_2011144024 && inner_x == BOARD_MID_2011144024) mode_2011144024 = 'N';
	else mode_2011144024 = '\n';
}

void second_side_marking_sub_2011144024(int inner_x, int inner_y, int *target_x, int *target_y, int add_x, int add_y) {
	*target_x = inner_x + add_x;
	*target_y = inner_y + add_y;
}

void second_side_marking_2011144024(int inner_x, int inner_y, int *target_x, int *target_y) {
	if (mode_2011144024 == 'A') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 3, 3);
	}
	else if (mode_2011144024 == 'B') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -3, 3);
	}
	else if (mode_2011144024 == 'C') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -3, -3);
	}
	else if (mode_2011144024 == 'D') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 3, -3);
	}
	else if (mode_2011144024 == 'E') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -3, 0);
	}
	else if (mode_2011144024 == 'W') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 3, 0);
	}
	else if (mode_2011144024 == 'S') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 0, -3);
	}
	else if (mode_2011144024 == 'N') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 0, 3);
	}
	else {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 1, 0);
	}
}

void second_mid_marking_2011144024(int inner_x, int inner_y, int *target_x, int *target_y) {
	if (mode_2011144024 == 'A') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 1, 0);
	}
	else if (mode_2011144024 == 'B') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 0, 1);
	}
	else if (mode_2011144024 == 'C') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -1, 0);
	}
	else if (mode_2011144024 == 'D') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 0, -1);
	}
	else if (mode_2011144024 == 'E') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -2, 0);
	}
	else if (mode_2011144024 == 'W') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 2, 0);
	}
	else if (mode_2011144024 == 'S') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 0, -2);
	}
	else if (mode_2011144024 == 'N') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 0, 2);
	}
	else {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 1, 0);
	}
}

void second_center_marking_2011144024(int inner_x, int inner_y, int *target_x, int *target_y) {
	if (mode_2011144024 == 'A') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -1, 0);
	}
	else if (mode_2011144024 == 'B') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 0, 1);
	}
	else if (mode_2011144024 == 'C') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 1, -1);
	}
	else if (mode_2011144024 == 'D') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -1, -1);
	}
	else if (mode_2011144024 == 'E') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -1, 0);
	}
	else if (mode_2011144024 == 'W') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 1, 0);
	}
	else if (mode_2011144024 == 'S') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, -1, 0);
	}
	else if (mode_2011144024 == 'N') {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 0, 1);
	}
	else {
		second_side_marking_sub_2011144024(inner_x, inner_y, target_x, target_y, 1, 0);
	}
}

void xClear_2011144024(int inner_x, int inner_y, int type) {
	game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].xMax = 0;
	if (inner_x < BOARD_SIZE_2011144024 - 1) if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x + 1].color == type) xClear_2011144024(inner_x + 1, inner_y, type);
	if (inner_x > 0) if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x - 1].color == type) xClear_2011144024(inner_x - 1, inner_y, type);
}

void yClear_2011144024(int inner_x, int inner_y, int type) {
	game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].yMax = 0;
	if (inner_y < BOARD_SIZE_2011144024 - 1) if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + 1) + inner_x].color == type) yClear_2011144024(inner_x, inner_y + 1, type);
	if (inner_y > 0) if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y - 1) + inner_x].color == type) yClear_2011144024(inner_x, inner_y - 1, type);
}

void uClear_2011144024(int inner_x, int inner_y, int type) {
	game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].uMax = 0;
	if (inner_x > 0 && inner_y < BOARD_SIZE_2011144024 - 1) if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + 1) + inner_x - 1].color == type) uClear_2011144024(inner_x - 1, inner_y + 1, type);
	if (inner_x < BOARD_SIZE_2011144024 - 1 && inner_y > 0) if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y - 1) + inner_x + 1].color == type) uClear_2011144024(inner_x + 1, inner_y - 1, type);
}

void dClear_2011144024(int inner_x, int inner_y, int type) {
	game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].dMax = 0;
	if (inner_x < BOARD_SIZE_2011144024 - 1 && inner_y < BOARD_SIZE_2011144024 - 1) if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + 1) + inner_x + 1].color == type) dClear_2011144024(inner_x + 1, inner_y + 1, type);
	if (inner_x > 0 && inner_y > 0) if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y - 1) + inner_x - 1].color == type) dClear_2011144024(inner_x - 1, inner_y - 1, type);
}

void xEnd_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y, int *count) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add < 0) {
		return;
	}
	else if (inner_x_add > BOARD_SIZE_2011144024 - 1) {
		return;
	}
	else {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == type) {
			*count += 1;
			xEnd_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y, count);
		}
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == -1 * type) {
			return;		// 끝이 막혀있음
		}
		else return;
	}
}

void yEnd_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y, int *count) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_y_add < 0) {
		return;
	}
	else if (inner_y_add > BOARD_SIZE_2011144024 - 1) {
		return;
	}
	else {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == type) {
			*count += 1;
			yEnd_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y, count);
		}
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == -1 * type) {
			return;		// 끝이 막혀있음
		}
		else return;
	}
}

void uEnd_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y, int *count) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add > BOARD_SIZE_2011144024 - 1 || inner_y_add < 0) {
		return;
	}
	else if (inner_x_add < 0 || inner_y_add > BOARD_SIZE_2011144024 - 1) {
		return;
	}
	else {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == type) {
			*count += 1;
			uEnd_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y, count);
		}
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == -1 * type) {
			return;		// 끝이 막혀있음
		}
		else return;
	}
}

void dEnd_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y, int *count) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add < 0 || inner_y_add < 0) {
		return;
	}
	else if (inner_x_add > BOARD_SIZE_2011144024 - 1 || inner_y_add > BOARD_SIZE_2011144024 - 1) {
		return;
	}
	else {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == type) {
			*count += 1;
			dEnd_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y, count);
		}
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == -1 * type) {
			return;		// 끝이 막혀있음
		}
		else return;
	}
}


void max_update_2011144024() {
	int PX, PY, PU, PD, MX, MY, MU, MD;
	for (int i = 0; i < BOARD_SIZE_2011144024; i++) {
		for (int j = 0; j < BOARD_SIZE_2011144024; j++) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color != 0) {
				PX = 0; PY = 0; PU = 0; PD = 0; MX = 0; MY = 0; MU = 0; MD = 0;
				xEnd_check_2011144024(j, i, game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color, 1, 0, &PX);
				xEnd_check_2011144024(j, i, game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color, -1, 0, &MX);
				game_board_2011144024[BOARD_SIZE_2011144024 * i + j].xMax = 1 + PX + MX;
				yEnd_check_2011144024(j, i, game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color, 0, 1, &PY);
				yEnd_check_2011144024(j, i, game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color, 0, -1, &MY);
				game_board_2011144024[BOARD_SIZE_2011144024 * i + j].yMax = 1 + PY + MY;
				uEnd_check_2011144024(j, i, game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color, 1, -1, &PU);
				uEnd_check_2011144024(j, i, game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color, -1, 1, &MU);
				game_board_2011144024[BOARD_SIZE_2011144024 * i + j].uMax = 1 + PU + MU;
				dEnd_check_2011144024(j, i, game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color, 1, 1, &PD);
				dEnd_check_2011144024(j, i, game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color, -1, -1, &MD);
				game_board_2011144024[BOARD_SIZE_2011144024 * i + j].dMax = 1 + PD + MD;
			}
		}
	}
}

void insert_xy_2011144024(int *inner_x, int *inner_y, int x_source, int y_source) {
	*inner_x = BOARD_MID_2011144024 + x_source;
	*inner_y = BOARD_MID_2011144024 + y_source;
}

void third_one_length_marking_2011144024(int *inner_x, int *inner_y) {
	if (mode_2011144024 == 'A') {
		insert_xy_2011144024(inner_x, inner_y, 1, 0);
	}
	else if (mode_2011144024 == 'B') {
		insert_xy_2011144024(inner_x, inner_y, -1, 0);
	}
	else if (mode_2011144024 == 'C') {
		insert_xy_2011144024(inner_x, inner_y, 0, -1);
	}
	else if (mode_2011144024 == 'D') {
		insert_xy_2011144024(inner_x, inner_y, 0, -1);
	}
	else if (mode_2011144024 == 'E') {
		insert_xy_2011144024(inner_x, inner_y, 1, 1);
	}
	else if (mode_2011144024 == 'W') {
		insert_xy_2011144024(inner_x, inner_y, -1, -1);
	}
	else if (mode_2011144024 == 'S') {
		insert_xy_2011144024(inner_x, inner_y, 1, 1);
	}
	else if (mode_2011144024 == 'N') {
		insert_xy_2011144024(inner_x, inner_y, 1, -1);
	}
}

void third_two_length_marking_2011144024(int inner_x, int inner_y, int *target_x, int *target_y) {
	if (mode_2011144024 == 'A') {
		if (BOARD_MID_2011144024 - inner_x > BOARD_MID_2011144024 - inner_y) insert_xy_2011144024(target_x, target_y, 0, 1);
		else if (BOARD_MID_2011144024 - inner_x < BOARD_MID_2011144024 - inner_y) insert_xy_2011144024(target_x, target_y, 1, 0);
		else insert_xy_2011144024(target_x, target_y, 1, -1);
	}
	else if (mode_2011144024 == 'B') {
		if (inner_x - BOARD_MID_2011144024 > BOARD_MID_2011144024 - inner_y) insert_xy_2011144024(target_x, target_y, 0, 1);
		else if (inner_x - BOARD_MID_2011144024 < BOARD_MID_2011144024 - inner_y) insert_xy_2011144024(target_x, target_y, -1, 0);
		else insert_xy_2011144024(target_x, target_y, -1, -1);
	}
	else if (mode_2011144024 == 'C') {
		if (inner_x - BOARD_MID_2011144024 > inner_y - BOARD_MID_2011144024) insert_xy_2011144024(target_x, target_y, -1, 0);
		else if (inner_x - BOARD_MID_2011144024 < inner_y - BOARD_MID_2011144024) insert_xy_2011144024(target_x, target_y, 0, -1);
		else insert_xy_2011144024(target_x, target_y, -1, 1);
	}
	else if (mode_2011144024 == 'D') {
		if (BOARD_MID_2011144024 - inner_x > inner_y - BOARD_MID_2011144024) insert_xy_2011144024(target_x, target_y, 1, 0);
		else if (BOARD_MID_2011144024 - inner_x < inner_y - BOARD_MID_2011144024) insert_xy_2011144024(target_x, target_y, 0, -1);
		else insert_xy_2011144024(target_x, target_y, 1, 1);
	}
	else if (mode_2011144024 == 'E') {
		insert_xy_2011144024(target_x, target_y, -2, 0);
	}
	else if (mode_2011144024 == 'W') {
		insert_xy_2011144024(target_x, target_y, 2, 0);
	}
	else if (mode_2011144024 == 'S') {
		insert_xy_2011144024(target_x, target_y, -2, 0);
	}
	else if (mode_2011144024 == 'N') {
		insert_xy_2011144024(target_x, target_y, 0, 2);
	}
}

int xBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add < 0) {
		return 0;
	}
	else if (inner_x_add > BOARD_SIZE_2011144024 - 1) {
		return 0;
	}
	else {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == type) {
			return xBlock_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y);
		}
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == -1 * type) {
			return 1;		// 끝이 막혀있음
		}
		else return 0;
	}
}

int yBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_y_add < 0) {
		return 0;
	}
	else if (inner_y_add > BOARD_SIZE_2011144024 - 1) {
		return 0;
	}
	else {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == type) {
			return yBlock_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y);
		}
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == -1 * type) {
			return 1;
		}
		else return 0;
	}
}

int uBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add > BOARD_SIZE_2011144024 - 1 || inner_y_add < 0) {
		return 0;
	}
	else if (inner_x_add < 0 || inner_y_add > BOARD_SIZE_2011144024 - 1) {
		return 0;
	}
	else {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == type) {
			return uBlock_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y);
		}
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == -1 * type) {
			return 1;
		}
		else return 0;
	}
}

int dBlock_check_2011144024(int inner_x, int inner_y, int type, int move_x, int move_y) {		// 돌 없으면 1리턴
	int inner_x_add = inner_x + move_x;
	int inner_y_add = inner_y + move_y;
	if (inner_x_add < 0 || inner_y_add < 0) {
		return 0;
	}
	else if (inner_x_add > BOARD_SIZE_2011144024 - 1 || inner_y_add > BOARD_SIZE_2011144024 - 1) {
		return 0;
	}
	else {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == type) {
			return dBlock_check_2011144024(inner_x_add, inner_y_add, type, move_x, move_y);
		}
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y_add + inner_x_add].color == -1 * type) {
			return 1;
		}
		else return 0;
	}
}

int check_between_2011144024(int A, int B, int type) {
	if (A == type && B == type) return 1;
	return 0;
}

int center_point_update_2011144024(int inner_x, int inner_y) {
	int result_x = BOARD_MID_2011144024 - inner_x;
	int result_y = BOARD_MID_2011144024 - inner_y;
	result_x = (result_x > 0) ? result_x : -1 * result_x;
	result_y = (result_y > 0) ? result_y : -1 * result_y;

	if (result_x > result_y) {
		return BOARD_MID_2011144024 - result_x;
	}
	else {
		return BOARD_MID_2011144024 - result_y;
	}
}

int change_absolute_value_2011144024(int target) {
	return (target > 0) ? target : -1 * target;
}

void hot_sopt_point_attack_making_2011144024(int inner_x, int inner_y, int type) {

	int PX = 0, PY = 0, PU = 0, PD = 0, MX = 0, MY = 0, MU = 0, MD = 0;
	xEnd_check_2011144024(inner_x, inner_y, type, 1, 0, &PX);
	xEnd_check_2011144024(inner_x, inner_y, type, -1, 0, &MX);
	yEnd_check_2011144024(inner_x, inner_y, type, 0, 1, &PY);
	yEnd_check_2011144024(inner_x, inner_y, type, 0, -1, &MY);
	uEnd_check_2011144024(inner_x, inner_y, type, 1, -1, &PU);
	uEnd_check_2011144024(inner_x, inner_y, type, -1, 1, &MU);
	dEnd_check_2011144024(inner_x, inner_y, type, 1, 1, &PD);
	dEnd_check_2011144024(inner_x, inner_y, type, -1, -1, &MD);

	int E_PX = 0, E_PY = 0, E_PU = 0, E_PD = 0, E_MX = 0, E_MY = 0, E_MU = 0, E_MD = 0;			// 한 칸 띄고
	if (PX == 0) xEnd_check_2011144024(inner_x + 1, inner_y, BLACK, 1, 0, &E_PX);
	if (MX == 0) xEnd_check_2011144024(inner_x - 1, inner_y, BLACK, -1, 0, &E_MX);
	if (PY == 0) yEnd_check_2011144024(inner_x, inner_y + 1, BLACK, 0, 1, &E_PY);
	if (MY == 0) yEnd_check_2011144024(inner_x, inner_y - 1, BLACK, 0, -1, &E_MY);
	if (PU == 0) uEnd_check_2011144024(inner_x + 1, inner_y - 1, BLACK, 1, -1, &E_PU);
	if (MU == 0) uEnd_check_2011144024(inner_x - 1, inner_y + 1, BLACK, -1, 1, &E_MU);
	if (PD == 0) dEnd_check_2011144024(inner_x + 1, inner_y + 1, BLACK, 1, 1, &E_PD);
	if (MD == 0) dEnd_check_2011144024(inner_x - 1, inner_y - 1, BLACK, -1, -1, &E_MD);


	if (inner_x == 0) {
		if (PX == 2 && MX == 0) {
			if (!(xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PX == 3 && MX == 0) {
			if (!(xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PX == 4 && MX == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PY == 2 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 0 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 1 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 3 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PY == 0 && MY == 3) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PY == 2 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PY == 1 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PY == 4 && MY == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 0 && MY == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 3 && MY == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 1 && MY == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 2 && MY == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PU == 2 && MU == 0) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PU == 3 && MU == 0) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PU == 4 && MU == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PD == 2 && MD == 0) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PD == 3 && MD == 0) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PD == 4 && MD == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
	
	}
	else if (inner_x == BOARD_SIZE_2011144024 - 1) {
		if (PX == 0 && MX == 2) {
			if (!(xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PX == 0 && MX == 3) {
			if (!(xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PX == 0 && MX == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PY == 2 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 0 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 1 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 3 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PY == 0 && MY == 3) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PY == 2 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PY == 1 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PY == 4 && MY == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 0 && MY == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 3 && MY == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 1 && MY == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 2 && MY == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PU == 0 && MU == 2) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PU == 0 && MU == 3) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PU == 0 && MU == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PD == 0 && MD == 2) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PD == 0 && MD == 3) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PD == 0 && MD == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

	}
	else if (inner_y == 0) {
		if (PX == 2 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 0 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 1 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 3 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PX == 0 && MX == 3) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PX == 2 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PX == 1 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PX == 4 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 10000;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		}
		else if (PX == 4 && MX == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 0 && MX == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 3 && MX == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 1 && MX == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 2 && MX == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PY == 2 && MY == 0) {
			if (!(yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PY == 3 && MY == 0) {
			if (!(yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PY == 4 && MY == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PU == 0 && MU == 2) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PU == 0 && MU == 3) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PU == 0 && MU == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		
		if (PD == 2 && MD == 0) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PD == 3 && MD == 0) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PD == 4 && MD == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
	}
	else if (inner_y == BOARD_SIZE_2011144024 - 1) {
		if (PX == 2 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 0 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 1 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 3 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PX == 0 && MX == 3) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PX == 2 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PX == 1 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PX == 4 && MX == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 0 && MX == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 3 && MX == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 1 && MX == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 2 && MX == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PY == 0 && MY == 2) {
			if (!(yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PY == 0 && MY == 3) {
			if (!(yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PY == 0 && MY == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PU == 2 && MU == 0) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PU == 3 && MU == 0) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PU == 4 && MU == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PD == 0 && MD == 2) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
		}
		else if (PD == 0 && MD == 3) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 251;
		}
		else if (PD == 0 && MD == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
	}
	else {

		// 차례대로 위 3개 3제작, 중간 4개 4제작, 마지막 5개 5제작
		if (PX == 2 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 0 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 1 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (E_PX == 1 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 1 && E_MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PX == 3 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PX == 0 && MX == 3) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PX == 2 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PX == 1 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PX == 4 && MX == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 0 && MX == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 3 && MX == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 1 && MX == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PX == 2 && MX == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;

		if (PY == 2 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 0 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 1 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PY == 3 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PY == 0 && MY == 3) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PY == 2 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PY == 1 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PY == 4 && MY == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 0 && MY == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 3 && MY == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 1 && MY == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PY == 2 && MY == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;


		if (PU == 2 && MU == 0) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PU == 0 && MU == 2) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PU == 1 && MU == 1) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PU == 3 && MU == 0) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PU == 0 && MU == 3) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PU == 2 && MU == 1) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PU == 1 && MU == 2) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PU == 4 && MU == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PU == 0 && MU == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PU == 3 && MU == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PU == 1 && MU == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PU == 2 && MU == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		

		if (PD == 2 && MD == 0) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PD == 0 && MD == 2) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PD == 1 && MD == 1) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 27;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 350;
		}
		else if (PD == 3 && MD == 0) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PD == 0 && MD == 3) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 301;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1500;
		}
		else if (PD == 2 && MD == 1) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PD == 1 && MD == 2) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -150;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 305;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1510;
		}
		else if (PD == 4 && MD == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PD == 0 && MD == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PD == 3 && MD == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PD == 1 && MD == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
		else if (PD == 2 && MD == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1000001;
	}
}

void hot_sopt_point_shield_making_2011144024(int inner_x, int inner_y, int type) {

	int PX = 0, PY = 0, PU = 0, PD = 0, MX = 0, MY = 0, MU = 0, MD = 0;
	xEnd_check_2011144024(inner_x, inner_y, type, 1, 0, &PX);
	xEnd_check_2011144024(inner_x, inner_y, type, -1, 0, &MX);
	yEnd_check_2011144024(inner_x, inner_y, type, 0, 1, &PY);
	yEnd_check_2011144024(inner_x, inner_y, type, 0, -1, &MY);
	uEnd_check_2011144024(inner_x, inner_y, type, 1, -1, &PU);
	uEnd_check_2011144024(inner_x, inner_y, type, -1, 1, &MU);
	dEnd_check_2011144024(inner_x, inner_y, type, 1, 1, &PD);
	dEnd_check_2011144024(inner_x, inner_y, type, -1, -1, &MD);


	if (inner_x == 0) {
		if (PX == 2 && MX == 0) {
			if (!(xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PX == 3 && MX == 0) {
			if (!(xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 4 && MX == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PY == 2 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 0 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 1 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 3 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PY == 0 && MY == 3) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PY == 2 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PY == 1 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PY == 4 && MY == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 0 && MY == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 3 && MY == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 1 && MY == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 2 && MY == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PU == 2 && MU == 0) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PU == 3 && MU == 0) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PU == 4 && MU == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PD == 2 && MD == 0) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PD == 3 && MD == 0) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PD == 4 && MD == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

	}
	else if (inner_x == BOARD_SIZE_2011144024 - 1) {
		if (PX == 0 && MX == 2) {
			if (!(xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PX == 0 && MX == 3) {
			if (!(xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 0 && MX == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PY == 2 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 0 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 1 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 3 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PY == 0 && MY == 3) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PY == 2 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PY == 1 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PY == 4 && MY == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 0 && MY == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 3 && MY == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 1 && MY == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 2 && MY == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PU == 0 && MU == 2) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PU == 0 && MU == 3) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PU == 0 && MU == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PD == 0 && MD == 2) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PD == 0 && MD == 3) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PD == 0 && MD == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

	}
	else if (inner_y == 0) {
		if (PX == 2 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 0 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 1 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 3 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PX == 0 && MX == 3) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PX == 2 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PX == 1 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PX == 4 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 330;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		}
		else if (PX == 4 && MX == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 0 && MX == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 3 && MX == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 1 && MX == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 2 && MX == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PY == 2 && MY == 0) {
			if (!(yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PY == 3 && MY == 0) {
			if (!(yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 4 && MY == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PU == 0 && MU == 2) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PU == 0 && MU == 3) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PU == 0 && MU == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PD == 2 && MD == 0) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PD == 3 && MD == 0) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PD == 4 && MD == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
	}
	else if (inner_y == BOARD_SIZE_2011144024 - 1) {
		if (PX == 2 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 0 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 1 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 3 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PX == 0 && MX == 3) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PX == 2 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PX == 1 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PX == 4 && MX == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 0 && MX == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 3 && MX == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 1 && MX == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 2 && MX == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PY == 0 && MY == 2) {
			if (!(yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PY == 0 && MY == 3) {
			if (!(yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 0 && MY == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PU == 2 && MU == 0) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PU == 3 && MU == 0) {
			if (!(uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PU == 4 && MU == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PD == 0 && MD == 2) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
		}
		else if (PD == 0 && MD == 3) {
			if (!(dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1))) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PD == 0 && MD == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
	}
	else {
		// 차례대로 위 3개 3제작, 중간 4개 4제작, 마지막 5개 5제작
		if (PX == 2 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 0 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 1 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PX == 3 && MX == 0) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PX == 0 && MX == 3) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PX == 2 && MX == 1) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PX == 1 && MX == 2) {
			if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) && xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (xBlock_check_2011144024(inner_x, inner_y, type, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, type, -1, 0)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PX == 4 && MX == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 0 && MX == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 3 && MX == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 1 && MX == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PX == 2 && MX == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;

		if (PY == 2 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 0 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 1 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PY == 3 && MY == 0) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PY == 0 && MY == 3) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PY == 2 && MY == 1) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PY == 1 && MY == 2) {
			if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) && yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (yBlock_check_2011144024(inner_x, inner_y, type, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, type, 0, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PY == 4 && MY == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 0 && MY == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 3 && MY == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 1 && MY == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PY == 2 && MY == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;


		if (PU == 2 && MU == 0) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PU == 0 && MU == 2) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PU == 1 && MU == 1) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PU == 3 && MU == 0) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PU == 0 && MU == 3) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PU == 2 && MU == 1) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PU == 1 && MU == 2) {
			if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) && uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (uBlock_check_2011144024(inner_x, inner_y, type, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, type, -1, 1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PU == 4 && MU == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PU == 0 && MU == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PU == 3 && MU == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PU == 1 && MU == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PU == 2 && MU == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;


		if (PD == 2 && MD == 0) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PD == 0 && MD == 2) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PD == 1 && MD == 1) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -9;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 5;
		}
		else if (PD == 3 && MD == 0) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PD == 0 && MD == 3) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1010;
		}
		else if (PD == 2 && MD == 1) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PD == 1 && MD == 2) {
			if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) && dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -153;
			else if (dBlock_check_2011144024(inner_x, inner_y, type, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, type, -1, -1)) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 33;
			else game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 1020;
		}
		else if (PD == 4 && MD == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PD == 0 && MD == 4) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PD == 3 && MD == 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PD == 1 && MD == 3) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
		else if (PD == 2 && MD == 2) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 9991;
	}
}

void hot_sopt_point_making_2011144024(int inner_x, int inner_y, int type) {
	hot_sopt_point_attack_making_2011144024(inner_x, inner_y, type);
	hot_sopt_point_shield_making_2011144024(inner_x, inner_y, -1 * type);
}

void new_shape_making_2011144024(int inner_x, int inner_y, int x_shift, int y_shift, int type) {
	if (y_shift == 0) {
		if (x_shift == 1 || x_shift == -1) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift - 1) + inner_x + x_shift].color == 0) {
				if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift - 2) + inner_x + x_shift].color != -1 * type && game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift - 2) + inner_x + x_shift + x_shift].color != -1 * type) {	// 막혀있지 않을 경우
					game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift - 1) + inner_x + x_shift].point += 27;
				}
			}
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + 1) + inner_x + x_shift].color == 0) {
				if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + 2) + inner_x + x_shift].color != -1 * type && game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + 2) + inner_x + x_shift + x_shift].color != -1 * type) {	// 막혀있지 않을 경우
					game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + 1) + inner_x + x_shift].point += 27;
				}
			}
		}
		else if (x_shift == 2 || x_shift == -2) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift - 1) + inner_x + x_shift].color == 0) {
				game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift - 1) + inner_x + x_shift].point += 28;
				if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + (x_shift / 2)].color == 0) game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift - 1) + inner_x + x_shift].point += 2;
			}
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + 1) + inner_x + x_shift].color == 0) {
				game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + 1) + inner_x + x_shift].point += 28;
				if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + (x_shift / 2)].color == 0) game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + 1) + inner_x + x_shift].point += 2;
			}
		}
	}
	else if (x_shift == 0) {
		if (y_shift == 1 || y_shift == -1) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift - 1].color == 0) {
				if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift - 2].color != -1 * type && game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + y_shift) + inner_x + x_shift - 2].color != -1 * type) {	// 막혀있지 않을 경우
					game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift - 1].point += 27;
				}
			}
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift + 1].color == 0) {
				if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift + 2].color != -1 * type && game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift + y_shift) + inner_x + x_shift + 2].color != -1 * type) {	// 막혀있지 않을 경우
					game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift + 1].point += 27;
				}
			}
		}
		else if (x_shift == 2 || x_shift == -2) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift - 1].color == 0) {
				game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift - 1].point += 28;
				if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift / 2) + inner_x + x_shift].color == 0) game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift - 1].point += 2;
			}
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift + 1].color == 0) {
				game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift + 1].point += 28;
				if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift / 2) + inner_x + x_shift].color == 0) game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift + 1].point += 2;
			}
		}
	}
	else if (x_shift == y_shift || x_shift == -1 * y_shift) {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift * 2) + inner_x + x_shift * 2].color == 0 && game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift * 3) + inner_x + x_shift * 3].color != -1 * type)
			game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift * 2) + inner_x + x_shift * 2].point += 28;
		if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift * 2) + inner_x].color == 0) {
			game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift * 2) + inner_x].point += 28;
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x].color != -1 * type) game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift * 2) + inner_x].point += 2;
		}
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x + x_shift * 2].color == 0) {
			game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x + x_shift * 2].point += 28;
			if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x + x_shift].color != -1 * type) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x + x_shift * 2].point += 2;
		}
	}
	else {
		if (change_absolute_value_2011144024(x_shift) > change_absolute_value_2011144024(y_shift)) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift / 2].color == 0) game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift / 2].point += 17;
			if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x + x_shift / 2].color == 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x + x_shift / 2].point += 17;
		}
		else {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift / 2) + inner_x + x_shift].color == 0) game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift / 2) + inner_x + x_shift].point += 17;
			if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift / 2) + inner_x].color == 0) game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift / 2) + inner_x].point += 17;
		}
	}
}

void raise_point_use_XY_2011144024(int inner_x, int inner_y, int x_shift, int y_shift, int raise, int type) {
	if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift].color == 0) {
		game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift].point += raise;
	}
	else if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + y_shift) + inner_x + x_shift].color == type) {
		new_shape_making_2011144024(inner_x, inner_y, x_shift, y_shift, type);
	}
}

void near_by_same_color_point_update_2011144024(int inner_x, int inner_y, int type) {
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, 0, 3, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 0, -1, 3, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 1, 0, 3, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 0, 1, 3, type);

	raise_point_use_XY_2011144024(inner_x, inner_y, -1, -1, 8, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 1, -1, 8, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 1, 1, 8, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, 1, 8, type);

	raise_point_use_XY_2011144024(inner_x, inner_y, -2, -1, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, -2, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 2, -1, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 1, -2, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -2, -1, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, -2, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -2, 1, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, 2, 4, type);

	raise_point_use_XY_2011144024(inner_x, inner_y, -2, 0, 9, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 0, -2, 9, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 2, 0, 9, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 0, 2, 9, type);
}

void near_by_other_color_point_update_2011144024(int inner_x, int inner_y, int type) {
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, 0, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 0, -1, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 1, 0, 4, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 0, 1, 4, type);

	raise_point_use_XY_2011144024(inner_x, inner_y, -1, -1, 8, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 1, -1, 8, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 1, 1, 8, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, 1, 8, type);

	raise_point_use_XY_2011144024(inner_x, inner_y, -2, -1, 5, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, -2, 5, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 2, -1, 5, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 1, -2, 5, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -2, -1, 5, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, -2, 5, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -2, 1, 5, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, -1, 2, 5, type);

	raise_point_use_XY_2011144024(inner_x, inner_y, -2, 0, 9, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 0, -2, 9, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 2, 0, 9, type);
	raise_point_use_XY_2011144024(inner_x, inner_y, 0, 2, 9, type);
}

void near_by_point_update_2011144024(int inner_x, int inner_y, int type) {
	if (inner_x > 2 && inner_x < BOARD_SIZE_2011144024 - 3 && inner_y > 2 && inner_y < BOARD_SIZE_2011144024 - 3) {
		if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].color == type) near_by_same_color_point_update_2011144024(inner_x, inner_y, type);
		else if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].color == -1 * type) near_by_other_color_point_update_2011144024(inner_x, inner_y, -1 * type);
	}
}

void long_linear_white_turn_2011144024(int inner_x, int inner_y) {

	int PX = 0, PY = 0, PU = 0, PD = 0, MX = 0, MY = 0, MU = 0, MD = 0;
	xEnd_check_2011144024(inner_x, inner_y, BLACK, 1, 0, &PX);
	xEnd_check_2011144024(inner_x, inner_y, BLACK, -1, 0, &MX);
	yEnd_check_2011144024(inner_x, inner_y, BLACK, 0, 1, &PY);
	yEnd_check_2011144024(inner_x, inner_y, BLACK, 0, -1, &MY);
	uEnd_check_2011144024(inner_x, inner_y, BLACK, 1, -1, &PU);
	uEnd_check_2011144024(inner_x, inner_y, BLACK, -1, 1, &MU);
	dEnd_check_2011144024(inner_x, inner_y, BLACK, 1, 1, &PD);
	dEnd_check_2011144024(inner_x, inner_y, BLACK, -1, -1, &MD);

	if (PX + MX >= 5) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 99999999;
	else if (PY + MY >= 5) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 99999999;
	else if (PU + MU >= 5) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 99999999;
	else if (PD + MD >= 5) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += 99999999;
}

void near_by_other_stone_blocking(int inner_x, int inner_y, int type) {
	int block_count_useing_other_stone_blocking = 0;
	
	if (game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x - 1].color == type && game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x + 1].color == -1 * type) block_count_useing_other_stone_blocking += 1;
	if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y - 1) + inner_x].color == type && game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + 1) + inner_x].color == -1 * type) block_count_useing_other_stone_blocking += 1;
	if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y - 1) + inner_x + 1].color == type && game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + 1) + inner_x - 1].color == -1 * type) block_count_useing_other_stone_blocking += 1;
	if (game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y - 1) + inner_x - 1].color == type && game_board_2011144024[BOARD_SIZE_2011144024 * (inner_y + 1) + inner_x + 1].color == -1 * type) block_count_useing_other_stone_blocking += 1;

	switch (block_count_useing_other_stone_blocking) {
	case 4 : game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -625; break;
	case 3: game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -125; break;
	case 2 : game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -25; break;
	case 1 : game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -5; break;
	}
}

void point_update_white_2011144024() {
	for (int i = 0; i < BOARD_SIZE_2011144024; i++) {
		for (int j = 0; j < BOARD_SIZE_2011144024; j++) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color == 0) {
				hot_sopt_point_making_2011144024(j, i, WHITE);
				near_by_other_stone_blocking(j, i, BLACK);
				long_linear_white_turn_2011144024(j, i);
			}
			else {
				game_board_2011144024[BOARD_SIZE_2011144024 * j + i].point += center_point_update_2011144024(j, i);
				near_by_point_update_2011144024(j, i, WHITE);
			}
		}
	}
}

void escape_foul_black_turn_2011144024(int inner_x, int inner_y) {

	int threethree_count_using_foul = 0;
	int fourfour_count_using_foul = 0;;
	int over_six_check_using_foul = 0;		// 빈돌에 대해 체크하니까 5를 체크

	int PX = 0, PY = 0, PU = 0, PD = 0, MX = 0, MY = 0, MU = 0, MD = 0;
	xEnd_check_2011144024(inner_x, inner_y, BLACK, 1, 0, &PX);
	xEnd_check_2011144024(inner_x, inner_y, BLACK, -1, 0, &MX);
	yEnd_check_2011144024(inner_x, inner_y, BLACK, 0, 1, &PY);
	yEnd_check_2011144024(inner_x, inner_y, BLACK, 0, -1, &MY);
	uEnd_check_2011144024(inner_x, inner_y, BLACK, 1, -1, &PU);
	uEnd_check_2011144024(inner_x, inner_y, BLACK, -1, 1, &MU);
	dEnd_check_2011144024(inner_x, inner_y, BLACK, 1, 1, &PD);
	dEnd_check_2011144024(inner_x, inner_y, BLACK, -1, -1, &MD);

	if (PX + MX >= 5) over_six_check_using_foul += 1;
	if (PY + MY >= 5) over_six_check_using_foul += 1;
	if (PU + MU >= 5) over_six_check_using_foul += 1;
	if (PD + MD >= 5) over_six_check_using_foul += 1;


	if (inner_x == 0) {
		if (PY == 2 && !yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1)) threethree_count_using_foul += 1;
		else if (MY == 2 && !yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1)) threethree_count_using_foul += 1;
	}
	else if (inner_x == BOARD_SIZE_2011144024 - 1) {
		if (PY == 2 && !yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1)) threethree_count_using_foul += 1;
		else if (MY == 2 && !yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1)) threethree_count_using_foul += 1;
	}
	else if (inner_y == 0) {
		if (PX == 2 && !xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0)) threethree_count_using_foul += 1;
		else if (MX == 2 && !xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0)) threethree_count_using_foul += 1;
	}
	else if (inner_y == BOARD_SIZE_2011144024 - 1) {
		if (PX == 2 && !xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0)) threethree_count_using_foul += 1;
		else if (MX == 2 && !xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0)) threethree_count_using_foul += 1;
	}
	else {
		int E_PX = 0, E_PY = 0, E_PU = 0, E_PD = 0, E_MX = 0, E_MY = 0, E_MU = 0, E_MD = 0;			// 한 칸 띄고
		if (PX == 0) xEnd_check_2011144024(inner_x + 1, inner_y, BLACK, 1, 0, &E_PX);
		if (MX == 0) xEnd_check_2011144024(inner_x - 1, inner_y, BLACK, -1, 0, &E_MX);
		if (PY == 0) yEnd_check_2011144024(inner_x, inner_y + 1, BLACK, 0, 1, &E_PY);
		if (MY == 0) yEnd_check_2011144024(inner_x, inner_y - 1, BLACK, 0, -1, &E_MY);
		if (PU == 0) uEnd_check_2011144024(inner_x + 1, inner_y - 1, BLACK, 1, -1, &E_PU);
		if (MU == 0) uEnd_check_2011144024(inner_x - 1, inner_y + 1, BLACK, -1, 1, &E_MU);
		if (PD == 0) dEnd_check_2011144024(inner_x + 1, inner_y + 1, BLACK, 1, 1, &E_PD);
		if (MD == 0) dEnd_check_2011144024(inner_x - 1, inner_y - 1, BLACK, -1, -1, &E_MD);

		// 3x3 check
		if (PX == 2 && MX == 0 && !(xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (PX == 0 && MX == 2 && !(xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (PX == 1 && MX == 1 && !(xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (E_PX == 2 && MX == 0 && !(xBlock_check_2011144024(inner_x + 1, inner_y, BLACK, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (E_PX == 1 && MX == 1 && !(xBlock_check_2011144024(inner_x + 1, inner_y, BLACK, 1, 0) || xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (PX == 0 && E_MX == 2 && !(xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || xBlock_check_2011144024(inner_x - 1, inner_y, BLACK, -1, 0))) threethree_count_using_foul += 1;
		else if (PX == 1 && E_MX == 1 && !(xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || xBlock_check_2011144024(inner_x - 1, inner_y, BLACK, -1, 0))) threethree_count_using_foul += 1;

		if (PY == 2 && MY == 0 && !(yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (PY == 0 && MY == 2 && !(yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (PY == 1 && MY == 1 && !(yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (E_PY == 2 && MY == 0 && !(yBlock_check_2011144024(inner_x, inner_y + 1, BLACK, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (E_PY == 1 && MY == 1 && !(yBlock_check_2011144024(inner_x, inner_y + 1, BLACK, 0, 1) || yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (PY == 0 && E_MY == 2 && !(yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || yBlock_check_2011144024(inner_x, inner_y - 1, BLACK, 0, -1))) threethree_count_using_foul += 1;
		else if (PY == 1 && E_MY == 1 && !(yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || yBlock_check_2011144024(inner_x, inner_y - 1, BLACK, 0, -1))) threethree_count_using_foul += 1;

		if (PU == 2 && MU == 0 && !(uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (PU == 0 && MU == 2 && !(uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (PU == 1 && MU == 1 && !(uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (E_PU == 2 && MU == 0 && !(uBlock_check_2011144024(inner_x + 1, inner_y - 1, BLACK, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (E_PU == 1 && MU == 1 && !(uBlock_check_2011144024(inner_x + 1, inner_y - 1, BLACK, 1, -1) || uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (PU == 0 && E_MU == 2 && !(uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || uBlock_check_2011144024(inner_x - 1, inner_y + 1, BLACK, -1, 1))) threethree_count_using_foul += 1;
		else if (PU == 1 && E_MU == 1 && !(uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || uBlock_check_2011144024(inner_x - 1, inner_y + 1, BLACK, -1, 1))) threethree_count_using_foul += 1;

		if (PD == 2 && MD == 0 && !(dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (PD == 0 && MD == 2 && !(dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (PD == 1 && MD == 1 && !(dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (E_PD == 2 && MD == 0 && !(dBlock_check_2011144024(inner_x + 1, inner_y + 1, BLACK, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (E_PD == 1 && MD == 1 && !(dBlock_check_2011144024(inner_x + 1, inner_y + 1, BLACK, 1, 1) || dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (PD == 0 && E_MD == 2 && !(dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || dBlock_check_2011144024(inner_x - 1, inner_y - 1, BLACK, -1, -1))) threethree_count_using_foul += 1;
		else if (PD == 1 && E_MD == 1 && !(dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || dBlock_check_2011144024(inner_x - 1, inner_y - 1, BLACK, -1, -1))) threethree_count_using_foul += 1;

		// 4x4 check
		if (PX == 3) fourfour_count_using_foul += 1;
		else if (MX == 3) fourfour_count_using_foul += 1;
		else if (PX == 2 && MX == 1 && (!xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || !xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (PX == 1 && MX == 2 && (!xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || !xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (E_PX == 3) fourfour_count_using_foul += 1;
		else if (E_MX == 3) fourfour_count_using_foul += 1;
		else if (E_PX == 2 && MX == 1 && (!xBlock_check_2011144024(inner_x + 1, inner_y, BLACK, 1, 0) || !xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (E_PX == 1 && MX == 2 && (!xBlock_check_2011144024(inner_x + 1, inner_y, BLACK, 1, 0) || !xBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (PX == 2 && E_MX == 1 && (!xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || !xBlock_check_2011144024(inner_x - 1, inner_y, BLACK, -1, 0))) fourfour_count_using_foul += 1;
		else if (PX == 1 && E_MX == 2 && (!xBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 0) || !xBlock_check_2011144024(inner_x - 1, inner_y, BLACK, -1, 0))) fourfour_count_using_foul += 1;

		if (PY == 3) fourfour_count_using_foul += 1;
		else if (MY == 3) fourfour_count_using_foul += 1;
		else if (PY == 2 && MY == 1 && (!yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || !yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (PY == 1 && MY == 2 && (!yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || !yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (E_PY == 3) fourfour_count_using_foul += 1;
		else if (E_MY == 3) fourfour_count_using_foul += 1;
		else if (E_PY == 2 && MY == 1 && (!yBlock_check_2011144024(inner_x, inner_y + 1, BLACK, 0, 1) || !yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (E_PY == 1 && MY == 2 && (!yBlock_check_2011144024(inner_x, inner_y + 1, BLACK, 0, 1) || !yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (PY == 2 && E_MY == 1 && (!yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || !yBlock_check_2011144024(inner_x, inner_y - 1, BLACK, 0, -1))) fourfour_count_using_foul += 1;
		else if (PY == 1 && E_MY == 2 && (!yBlock_check_2011144024(inner_x, inner_y, BLACK, 0, 1) || !yBlock_check_2011144024(inner_x, inner_y - 1, BLACK, 0, -1))) fourfour_count_using_foul += 1;

		if (PU == 3) fourfour_count_using_foul += 1;
		else if (MU == 3) fourfour_count_using_foul += 1;
		else if (PU == 2 && MU == 1 && (!uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || !uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (PU == 1 && MU == 2 && (!uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || !uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (E_PU == 3) fourfour_count_using_foul += 1;
		else if (E_MU == 3) fourfour_count_using_foul += 1;
		else if (E_PU == 2 && MU == 1 && (!uBlock_check_2011144024(inner_x + 1, inner_y - 1, BLACK, 1, -1) || !uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (E_PU == 1 && MU == 2 && (!uBlock_check_2011144024(inner_x + 1, inner_y - 1, BLACK, 1, -1) || !uBlock_check_2011144024(inner_x, inner_y, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (PU == 2 && E_MU == 1 && (!uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || !uBlock_check_2011144024(inner_x - 1, inner_y + 1, BLACK, -1, 1))) fourfour_count_using_foul += 1;
		else if (PU == 1 && E_MU == 2 && (!uBlock_check_2011144024(inner_x, inner_y, BLACK, 1, -1) || !uBlock_check_2011144024(inner_x - 1, inner_y + 1, BLACK, -1, 1))) fourfour_count_using_foul += 1;

		if (PD == 3) fourfour_count_using_foul += 1;
		else if (MD == 3) fourfour_count_using_foul += 1;
		else if (PD == 2 && MD == 1 && (!dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || !dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (PD == 1 && MD == 2 && (!dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || !dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (E_PD == 3) fourfour_count_using_foul += 1;
		else if (E_MD == 3) fourfour_count_using_foul += 1;
		else if (E_PD == 2 && MD == 1 && (!dBlock_check_2011144024(inner_x + 1, inner_y + 1, BLACK, 1, 1) || !dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (E_PD == 1 && MD == 2 && (!dBlock_check_2011144024(inner_x + 1, inner_y + 1, BLACK, 1, 1) || !dBlock_check_2011144024(inner_x, inner_y, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (PD == 2 && E_MD == 1 && (!dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || !dBlock_check_2011144024(inner_x - 1, inner_y - 1, BLACK, -1, -1))) fourfour_count_using_foul += 1;
		else if (PD == 1 && E_MD == 2 && (!dBlock_check_2011144024(inner_x, inner_y, BLACK, 1, 1) || !dBlock_check_2011144024(inner_x - 1, inner_y - 1, BLACK, -1, -1))) fourfour_count_using_foul += 1;
	}



	if (over_six_check_using_foul > 0) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -99999999;		// 6목이 제일 위험
	else if (fourfour_count_using_foul > 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -999999;
	else if (threethree_count_using_foul > 1) game_board_2011144024[BOARD_SIZE_2011144024 * inner_y + inner_x].point += -999999;
}


void point_update_black_2011144024() {
	for (int i = 0; i < BOARD_SIZE_2011144024; i++) {
		for (int j = 0; j < BOARD_SIZE_2011144024; j++) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color == 0) {
				hot_sopt_point_making_2011144024(j, i, BLACK);
				near_by_other_stone_blocking(j, i, WHITE);
				escape_foul_black_turn_2011144024(j, i);
			}
			else {
				game_board_2011144024[BOARD_SIZE_2011144024 * j + i].point += center_point_update_2011144024(j, i);
				near_by_point_update_2011144024(j, i, BLACK);
			}
		}
	}
}

void max_point_xy_locate_return_2011144024(int *target_x, int *target_y) {
	int max_point = -1;
	int x, y;
	for (int i = 0; i < BOARD_SIZE_2011144024; i++) {
		for (int j = 0; j < BOARD_SIZE_2011144024; j++) {
			if (game_board_2011144024[BOARD_SIZE_2011144024 * i + j].color == 0) {
				if (max_point < game_board_2011144024[BOARD_SIZE_2011144024 * i + j].point) {
					max_point = game_board_2011144024[BOARD_SIZE_2011144024 * i + j].point;
					x = j;
					y = i;
				}
			}

		}
	}
	*target_x = x;
	*target_y = y;
}

void WhiteAttack_2011144024(int *x, int *y)
{
	// 3x3 가능, 4x4 가능, 6목 가능하되 승패 영향x

	player_white_turn_2011144024 += 1;

	int white_x, white_y;

	if (player_white_turn_2011144024 == 2) {
		if (last_stone_x_2011144024 < 3 || last_stone_y_2011144024 < 3 || last_stone_x_2011144024 >= BOARD_SIZE_2011144024 - 3 || last_stone_y_2011144024 >= BOARD_SIZE_2011144024 - 3) {
			second_side_marking_2011144024(last_stone_x_2011144024, last_stone_y_2011144024, &white_x, &white_y);
		}
		else if (last_stone_x_2011144024 >= BOARD_MID_2011144024 - 1 && last_stone_x_2011144024 <= BOARD_MID_2011144024 + 1 && last_stone_y_2011144024 >= BOARD_MID_2011144024 - 1 && last_stone_y_2011144024 <= BOARD_MID_2011144024 + 1) {
			second_center_marking_2011144024(last_stone_x_2011144024, last_stone_y_2011144024, &white_x, &white_y);
		}
		else {
			second_mid_marking_2011144024(last_stone_x_2011144024, last_stone_y_2011144024, &white_x, &white_y);
		}
	}
	else {
		reset_point_2011144024();
		point_update_white_2011144024();
		max_point_xy_locate_return_2011144024(&white_x, &white_y);
	}

	*x = white_x;
	*y = white_y;

	last_stone_x_2011144024 = white_x;
	last_stone_y_2011144024 = white_y;

	if (mode_2011144024 == '\n') catch_mode_2011144024(white_x, white_y);

	took_stone_2011144024(white_x, white_y, WHITE);
	max_update_2011144024();
	// blocked_stone_max_claer(white_x, white_y, WHITE);

}
void WhiteDefence_2011144024(int x, int y)
{
	//흰돌 1
	//검은돌 -1

	player_white_turn_2011144024 += 1;

	last_stone_x_2011144024 = x;
	last_stone_y_2011144024 = y;

	if (mode_2011144024 == '\n') catch_mode_2011144024(x, y);

	took_stone_2011144024(x, y, BLACK);
	max_update_2011144024();
	// blocked_stone_max_claer(x, y, BLACK);
}
void AttackBlack_2011144024(int *x, int *y)
{
	// 3x3, 4x4, 6목 불가

	player_black_turn_2011144024 += 1;

	int black_x, black_y;

	if (player_black_turn_2011144024 == 1) { black_x = 9; black_y = 9; }
	else if (player_black_turn_2011144024 == 3) {
		if (last_stone_x_2011144024 >= BOARD_MID_2011144024 - 1 && last_stone_x_2011144024 <= BOARD_MID_2011144024 + 1 && last_stone_y_2011144024 >= BOARD_MID_2011144024 - 1 && last_stone_y_2011144024 <= BOARD_MID_2011144024 + 1) {
			third_one_length_marking_2011144024(&black_x, &black_y);
		}
		else if (last_stone_x_2011144024 >= BOARD_MID_2011144024 - 2 && last_stone_x_2011144024 <= BOARD_MID_2011144024 + 2 && last_stone_y_2011144024 >= BOARD_MID_2011144024 - 2 && last_stone_y_2011144024 <= BOARD_MID_2011144024 + 2) {
			third_two_length_marking_2011144024(last_stone_x_2011144024, last_stone_y_2011144024, &black_x, &black_y);
		}
		else {
			reset_point_2011144024();
			point_update_black_2011144024();
			max_point_xy_locate_return_2011144024(&black_x, &black_y);
		}
	}
	else {
		reset_point_2011144024();
		point_update_black_2011144024();
		max_point_xy_locate_return_2011144024(&black_x, &black_y);
	}



	*x = black_x;
	*y = black_y;


	last_stone_x_2011144024 = black_x;
	last_stone_y_2011144024 = black_y;

	if (mode_2011144024 == '\n') catch_mode_2011144024(black_x, black_y);

	took_stone_2011144024(black_x, black_y, BLACK);
	max_update_2011144024();
	// blocked_stone_max_claer(black_x, black_y, BLACK);
}
void DefenceBlack_2011144024(int x, int y)
{
	//흰돌 1
	//검은돌 -1

	player_black_turn_2011144024 += 1;


	last_stone_x_2011144024 = x;
	last_stone_y_2011144024 = y;

	if (mode_2011144024 == '\n') catch_mode_2011144024(x, y);

	took_stone_2011144024(x, y, WHITE);
	max_update_2011144024();
	// blocked_stone_max_claer(x, y, WHITE);
}