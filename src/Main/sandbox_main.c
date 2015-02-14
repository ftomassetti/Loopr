#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "SandBox_pri.h"
#include "MEM.h"
#include "UTL.h"
#include "DBG.h"
#include "Interfaces.h"

int main(int argc, char **argv)
{
	FILE *fp = NULL;
	ByteContainer *container;
	ExeEnvironment *env;

	setlocale(LC_ALL, "");
#if 1

	if (argc >= 2) {
		fp = fopen(argv[1], "r+b");
		if (fp == NULL) {
			DBG_panic(("Cannot open file %s\n", argv[1]));
		}
	} else {
		fprintf(stdout, "Loopr SandBox [test version]\n"
						"Bug report: https://github.com/Ivory-Next/Loopr\n");
		exit(0);
	}

	container = ISerialize_read_byte_container(fp);
	fclose(fp);

	env = Coding_init_exe_env(container, LPR_ANYTHING);

	Loopr_execute(env, LPR_True);
#endif

Walle_update_alive_period();
Walle_gcollect();
Walle_dispose_environment(env);
Walle_dispose_byte_container(container, LPR_False);
MEM_dump_blocks(stderr);

	return 0;
}