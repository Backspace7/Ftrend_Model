#include <stdlib.h>
#include <libpq-fe.h>

#define LEN_ERROR	1000

struct BddResult
{
  PGresult *result;	
  int rows;
  int fields;
};

PGconn *CONN;

char bdderror[LEN_ERROR];

int BddConn(char *params);
void BddDisc();
void BddClear(struct BddResult *res);
char *BddError();
struct BddResult *BddSql(char *sql);
char *BddValue(struct BddResult *res, int row, int field);
struct BddResult *BddCommit();
