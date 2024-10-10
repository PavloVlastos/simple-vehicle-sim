CC=clang-13
LINK=llvm-link-13
CFLAGS += -emit-llvm -g -O0\
	-I ../modules \
	-I ../modules/common \
	-I ../modules/lin_alg \
	-I /snap/klee/10/usr/local/include/klee 

LDFLAGS += -lm

SRC_DIR = ../modules
BC_DIR = ./bc

all: linked_lin_alg.bc

$(BC_DIR)/KT_lin_alg.bc: KT_lin_alg.c 
	@mkdir -p $(BC_DIR)
	$(CC) $(CFLAGS) -c KT_lin_alg.c -o $(BC_DIR)/KT_lin_alg.bc

$(BC_DIR)/lin_alg.bc: $(SRC_DIR)/lin_alg/lin_alg.c
	@mkdir -p $(BC_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/lin_alg/lin_alg.c -o $(BC_DIR)/lin_alg.bc

$(BC_DIR)/common.bc: $(SRC_DIR)/common/common.c
	@mkdir -p $(BC_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/common/common.c -o $(BC_DIR)/common.bc

linked_lin_alg.bc: $(BC_DIR)/KT_lin_alg.bc $(BC_DIR)/lin_alg.bc $(BC_DIR)/common.bc
	@mkdir -p $(BC_DIR)
	$(LINK) $(BC_DIR)/KT_lin_alg.bc $(BC_DIR)/lin_alg.bc $(BC_DIR)/common.bc -o linked_lin_alg.bc

clean:
	rm -f *.bc *.o
	rm -rf klee-*
	rm -rf bc
