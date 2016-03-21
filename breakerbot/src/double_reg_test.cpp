#include <stdlib.h>
#include <stdio.h>

union double_reg {
    uint8_t     u_eight[2];
    uint16_t    u_sixteen;
    struct      {uint8_t upper, lower;};
};

int main() {
    double_reg test;
    // test.lower = 0x12;
    // test.upper = 0x34;
    // test.u_eight[1] = 0x12;
    // test.u_eight[0] = 0x34;
    test.u_sixteen = 0x1234;
    printf("LOWER = 0x%02x, UPPER = 0x%02x\n", test.lower, test.upper);
    printf("u_eight[1] = 0x%02x, u_eight[0] = 0x%02x\n", test.u_eight[1], test.u_eight[0]);
    printf("test.u_sixteen = 0x%04x decimal = %d\n", test.u_sixteen, test.u_sixteen);
    int start = 1231;
    int module
    return 0;
}
