#include "client.h"
#include <commons/log.h>
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
    
    // Loggeamos el valor de config
    if (valor != NULL) {
        log_info(logger, "Valor de CLAVE: %s", valor);
    } else {
        log_error(logger, "No se pudo obtener el valor de CLAVE");
    }


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "CLIENT", true, LOG_LEVEL_INFO);
    
    if (nuevo_logger == NULL) {
        printf("No se pudo crear el logger\n");
        exit(1);
    }

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
    
    if (nuevo_config == NULL) {
        printf("No se pudo crear la configuración\n");
        abort();
    }

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	 char* leido;
    
    while ((leido = readline("> ")) != NULL && strcmp(leido, "") != 0) {
        log_info(logger, "%s", leido);
        free(leido);
    }
    
    free(leido);
    log_info(logger, "Finalizando lectura de consola");
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while ((leido = readline("> ")) != NULL && strcmp(leido, "") != 0) {
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);
    }

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
    enviar_paquete(paquete, conexion);
    eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if (logger != NULL)
        log_destroy(logger);

    if (config != NULL)
        config_destroy(config);

    liberar_conexion(conexion);
}
