#ifndef __MISC_H__
#define __MISC_H__

make_helper(nop);
make_helper(int3);
make_helper(lea);
make_helper(leave);
make_helper(ret);
make_helper(cltd);
make_helper(cld);
make_helper(movsb);
make_helper(movsw);
make_helper(stosb);
make_helper(stosw);
make_helper(cmpsb);
make_helper(cmpsw);
make_helper(scasb);
make_helper(scasw);

#endif
