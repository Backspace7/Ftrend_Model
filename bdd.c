#include "bdd.h"

int BddConn(char *params)
{
  CONN = PQconnectdb(params);

  if(PQstatus(CONN) == CONNECTION_BAD)
  {
    snprintf(bdderror, LEN_ERROR, "%s", PQerrorMessage(CONN));
    return(-1);
  }

  return(1);
}

void BddDisc()
{
  PQfinish(CONN);

  return;
}

void BddClear(struct BddResult *result)
{
  PQclear(result->result);
  free(result);

  return;
}

char *BddError()
{
  return(bdderror);
}

struct BddResult *BddSql(char *sql)
{
  PGresult *res;	
  struct BddResult *result;	

  res = PQexec(CONN, sql);

  if(PQresultStatus(res) != PGRES_TUPLES_OK)
  {
    PQclear(res);
    return(NULL);
  }

  result = (struct BddResult *)malloc(sizeof(struct BddResult));
  
  result->result = res;
  result->rows   = PQntuples(res);
  result->fields = PQnfields(res);	  
		  
  return(result);
}

char *BddValue(struct BddResult *result, int row, int field)
{
  return(PQgetvalue(result->result, row, field));
}

/*struct BddResult *BddCommit()
{
  struct BddResult *result;

  result->result = (PGresult *)malloc(sizeof(result->result));
  result->result = PQexec(CONN, "COMMIT");

  return(result);
}*/












