#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/inline_c.h"
#include "psyq/inline_c_gas.h"

void func_8001BBD8(MATRIX *arg0) {
    gte_SetRotMatrix(&D_80061E88);
    gte_ldclmv(&arg0->m[0][0]);
    gte_rtir();
    gte_stclmv(&D_80061E88.m[0][0]);
    gte_ldclmv(&arg0->m[0][1]);
    gte_rtir();
    gte_stclmv(&D_80061E88.m[0][1]);
    gte_ldclmv(&arg0->m[0][2]);
    gte_rtir();
    gte_stclmv(&D_80061E88.m[0][2]);
    gte_SetTransMatrix(&D_80061E88);
    gte_ldlv0(&arg0->t[0]);
    gte_rtv0tr();
    gte_stlvl(&D_80061E88.t[0]);
    gte_SetRotMatrix(&D_80061E88);
    gte_SetTransMatrix(&D_80061E88);
}
