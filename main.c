
//===--- Uso básico para libpq-fe.h ---------------------------------------===//
//
// Autor: Rafael Fernando Garcia Sagastume
// Escuintla, Guatemala
//
// El archivo contiene ejemplos básicos de conexión, consulta e iteración.
// Utiliza los conceptos y utilerias de libpq-fe para Postgresql.
//
// para Linux: 	Instalar la liberia libpq-fe, postgresql, y una base de datos 
// 				configurada para realizar las pruebas o consultas.
// string de conexión: dbname=dbmodulo host=localhost user=username password=password
//				debes modificarlo para tu uso correcto.
// compilacion: gcc main.c -lpq -o main
// 
// Copyright (c) 2017 Rafael Fernando Garcia Sagastume.
//
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include <libpq-fe.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	/* code */
	printf("Iniciando Proceso en C...\n");
	PGconn *conn;
	PGresult *res;
	int res_count;
	int res_col;
	int row;
	int col;
	int version_server;	

	conn = PQconnectdb("dbname=dbmodulo host=localhost user=username password=password");
	if (PQstatus(conn) == CONNECTION_BAD)
	{
		printf("No se ha logrado conectar a la base de datos..");
		return 0;
	}


	//vemos la version del servidor
	version_server = PQserverVersion(conn);
	printf("version server: %d\n", version_server);


	//primera consulta postgresql con c
	res = PQexec(conn, "SELECT id_categoria, descripcion FROM categorias ORDER BY id_categoria");
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		printf("No se encontro data en la consulta!");
		return 0;
	}

	res_count = PQntuples(res);
	printf("cantidad de tuplas en la consulta: %d \n", res_count);

	res_col = PQnfields(res);
	printf("Cantidad de columnas de la consulta: %d\n", res_col);


	//impresion de nombre de columnas
	for (int i = 0; i < res_col; ++i)
	{
		printf("%s\t", PQfname(res, i));
	}
	printf("\n");

	//impresion de datos de la consulta
	for (row = 0; row < res_count; ++row)
	{
		for (col = 0; col < res_col; ++col)
		{
			printf("%s\t\t", PQgetvalue(res, row, col));
		}
		printf("\n");
	}

	
	//Ejemplo para el uso de transacciones con ejecuciones independientes
	res = PQexec(conn, "start transaction");
	res = PQexec(conn, "DELETE FROM detalles where id_detalle = 58");
	res = PQexec(conn, "rollback;");
	

	//limpia variables y suelta la coneccion
	PQclear(res);
	PQfinish(conn);

	return 0;
}