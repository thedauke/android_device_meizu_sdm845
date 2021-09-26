#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "edify/expr.h"
#include "otautil/error_code.h"

#define RPMB_PATH "/dev/mz_rpmb_ctl"

#define CMD 0x2a761729

#define UNLOCK_DEVICE 0x21560780
#define LOST_MODE 0x21560781
#define LOCK_MODE 0x21560782
#define GET_RSA_SEED 0x21560783
#define ROOT_DEVICE 0x21560784
#define UNROOT_DEVICE 0x21560785
#define SLOT_WRITE 0x21560786
#define SLOT_READ 0x21560787
#define GET_VERSION 0x21560788
#define LOCK_STATE 0x21560789
#define GET_ROOT_STATE 0x21560790
#define READ_RAC 0x21560791
#define WRITE_RAC 0x21560792
#define CLEAR_RAC 0x21560793
#define VERIFY_DEVICE 0x215607a2

Value* clear_mz_root(const char* name, State* state, const std::vector<std::unique_ptr<Expr>>&) {
  int fd, ret;
  unsigned long arg;

  fd = open(RPMB_PATH, O_RDWR);
  if (fd < 0) {
    return ErrorAbort(state, kFileOpenFailure, "%s() failed to open mz_rpmb: %d", name, errno);
  } else {
    arg = UNROOT_DEVICE;
    ret = ioctl(fd, CMD, &arg);
    if (ret != 0) {
      return ErrorAbort(state, kVendorFailure, "%s() failed to clear root: %d", name, ret);
    } else {
      arg = CLEAR_RAC;
      ret = ioctl(fd, CMD, &arg);
      if (ret != 0) {
        return ErrorAbort(state, kVendorFailure, "%s() failed to clear rac: %d", name, ret);
      }
    }
    close(fd);
  }

  return StringValue(strdup(""));
}

void Register_librecovery_updater_meizu_sdm845() {
  RegisterFunction("meizu_sdm845.clear_mz_root", clear_mz_root);
}