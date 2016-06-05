#include "index.h"
#include "sever.h"
#include "lz.h"
#include "task.h"
#include "check.h"
int main(int argc, char *argv[]) {
	wordexpand::Check checker;
	checker.CheckTask();
	return 0;
}