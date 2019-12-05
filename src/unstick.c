#include <stdio.h>
#include <seccomp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define NELEMS(x) (sizeof(x) / sizeof(*(x)))

int main(void) {
  scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
  if (!ctx) {
    perror("Could not set up seccomp context");
    return 1;
  }

  int perms[] = { S_ISUID, S_ISGID };
  for (size_t i = 0; i < NELEMS(perms); i++) {
    const scmp_datum_t perm = perms[i];
    if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(0), SCMP_SYS(chmod), 1,
			 SCMP_A1(SCMP_CMP_MASKED_EQ, perm, perm)) != 0) {
      perror("Could not add seccomp rule");
      goto error;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(0), SCMP_SYS(fchmod), 1,
			 SCMP_A1(SCMP_CMP_MASKED_EQ, perm, perm)) != 0) {
      perror("Could not add seccomp rule");
      goto error;
    }
    if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(0), SCMP_SYS(fchmodat), 1,
			 SCMP_A2(SCMP_CMP_MASKED_EQ, perm, perm)) != 0) {
      perror("Could not add seccomp rule");
      goto error;
    }
  }

  if (seccomp_load(ctx) != 0) {
    perror("Could not load seccomp program");
    goto error;
  }

  seccomp_release(ctx);
  return 0;

 error:
  seccomp_release(ctx);
  return 1;
}
