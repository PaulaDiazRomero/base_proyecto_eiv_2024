#include "unity.h"
#include "evento.h" // Para compilar evento.c
#include "cola_eventos.h"
#include "mef.h"

typedef struct Prueba Prueba;
struct Prueba{
    struct{
        unsigned inicializa;
        unsigned salida;
        unsigned entrada;
        unsigned b;
    }a;
    struct{
        unsigned salida;
        unsigned entrada;
        unsigned b;
        unsigned c;
    }b;
};

static Mef dut;

static Prueba prueba;

enum MisEventos{
    Mensaje_B = Mensaje_USUARIO,
    Mensaje_C
};
static struct{
    Evento evtB;
    Evento evtC; 
}const eventos= {
    .evtB={.mensaje=Mensaje_B},
    .evtC={.mensaje=Mensaje_C}
};


static void a(Mef *m,const Evento *e);
static void b(Mef *m,const Evento *e);

static void a(Mef *m,const Evento *e)
{
    (void)m;
    switch(Evento_obtMensaje(e)){
    case Mensaje_INICIALIZA:
        TEST_ASSERT_EQUAL_UINT_MESSAGE(prueba.a.inicializa,prueba.a.entrada,"La inicializacion se ejecuta antes que la entrada al estado");
        prueba.a.inicializa++;
    break;case Mensaje_ENTRADA:
        prueba.a.entrada++;
    break;case Mensaje_SALIDA:
        prueba.a.salida++;
    break;case Mensaje_B:
        prueba.a.b++;
        Mef_transiciona(m,b);
    break;default:
    }
}

static void b(Mef *m,const Evento *e)
{
    (void)m;
    switch (Evento_obtMensaje(e))
    {
    case Mensaje_ENTRADA:
        prueba.b.entrada++;
    break;case Mensaje_SALIDA:
        prueba.b.salida++;
    break;case Mensaje_B:
        prueba.b.b++;
    break;case Mensaje_C:
        prueba.b.c++;
        Mef_transiciona(m,b);
    break;default:
    break;
    }
}
void setUp(void)
{
    prueba = (Prueba){};
    Mef_init(&dut,a);
}

void tearDown(void)
{
}

void test_mefInicializa(void)
{
    Mef_ejecuta(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.entrada,"Debe recibir evento ENTRADA al entrar en estado");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.inicializa,"Estado inicial debe recibir evento INICIALIZA");
}

void test_mefTransicion(void)
{
    Mef_ejecuta(&dut);
    TEST_ASSERT_TRUE(Mef_recibeEvento(&dut,&eventos.evtB));
    Mef_ejecuta(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.b,"Debe procesar el evento B");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.salida,"Debe procesar evento SALIDA al salir del estado a");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.b.entrada,"Debe procesar evento ENTRADA al entrar al estado b");
}

void test_mefTransicionInterna(void)
{
    Mef_ejecuta(&dut);
    Mef_recibeEvento(&dut,&eventos.evtB);
    Mef_ejecuta(&dut);
    TEST_ASSERT_TRUE(Mef_recibeEvento(&dut,&eventos.evtB));
    Mef_ejecuta(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.b.b,"Debe procesar el evento B");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(0,prueba.b.salida,"La transicion interna no genera salida ni entrada");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.b.entrada,"La transicion interna no genera salida ni entrada");
}
void test_mefTransicionExternaAlMismoEstado(void)
{
    Mef_ejecuta(&dut);
    Mef_recibeEvento(&dut,&eventos.evtB);
    Mef_ejecuta(&dut);
    Mef_recibeEvento(&dut,&eventos.evtC);
    Mef_ejecuta(&dut);

    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.b.c,"Debe recibir el evento C");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.b.salida,"Debe procesar evento SALIDA al salir del estado a");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(2,prueba.b.entrada,"Debe procesar evento ENTRADA al entrar al estado b");
}

void test_mefFinaliza(void)
{
    Mef_ejecuta(&dut);
    Mef_finaliza(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.salida,"Debe procesar evento SALIDA al finalizar la máquina");
    Mef_finaliza(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.salida,"Posteriores finalizaciones no tienen otro efecto");
}
void test_mefReiniciaMaquinaFinalizada(void)
{
    Mef_ejecuta(&dut);
    Mef_finaliza(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.salida,"Debe procesar evento SALIDA al finalizar la máquina");
    Mef_ejecuta(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(2,prueba.a.inicializa,"Vuelve a inicializar");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(2,prueba.a.entrada,"Vuelve a ingresar al estado inicial");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.salida,"No sale del estado");
}
void test_mefComoObservadorEventos(void)
{
    ObservadorEventos *obs = Mef_obtObservador(&dut);
    TEST_ASSERT_NOT_NULL_MESSAGE(obs,"La interfaz observador no puede ser un puntero nulo");
    Mef_ejecuta(&dut);
    TEST_ASSERT_TRUE(ObservadorEventos_recibeEvento(obs,&eventos.evtB));
    Mef_ejecuta(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.b,"Debe procesar el evento B");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.a.salida,"Debe procesar evento SALIDA al salir del estado a");
    TEST_ASSERT_EQUAL_UINT_MESSAGE(1,prueba.b.entrada,"Debe procesar evento ENTRADA al entrar al estado b");

}


static void x(Mef *m,const Evento *e)
{
    switch (Evento_obtMensaje(e))
    {
    case Mensaje_INICIALIZA:
        TEST_ASSERT_TRUE_MESSAGE(Mef_enviaEvento(m,&eventos.evtB),"Debe poder enviar un evento");
    break;default:
    break;
    }
}



void test_mefComoFuenteEventosMultiplesObservadores(void)
{
    static Mef aux,otra;
    Mef_init(&otra,a);
    Mef_init(&aux,x);
    Mef_registraObservador(&aux,Mef_obtObservador(&dut));
    TEST_ASSERT_TRUE_MESSAGE(Mef_registraObservador(&aux,Mef_obtObservador(&otra)),"Debe poder registrar varios observadores de eventos");
    Mef_ejecuta(&dut);
    Mef_ejecuta(&otra);
    Mef_ejecuta(&aux);
    Mef_ejecuta(&dut);
    Mef_ejecuta(&otra);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(2,prueba.a.b,"Ambos observadores deben recibir el evento B");
}

static void y(Mef *m,const Evento *e)
{
    switch (Evento_obtMensaje(e))
    {
    case Mensaje_INICIALIZA:
        TEST_ASSERT_FALSE_MESSAGE(Mef_enviaEvento(m,&eventos.evtB),"No debe poder enviar un evento sin receptores");
    break;default:
    break;
    }
}

void test_mefComoFuenteEventosDesregistraObservador(void)
{
    Mef aux;
    Mef_init(&aux,y);
    Mef_registraObservador(&aux,Mef_obtObservador(&dut));
    Mef_ejecuta(&dut);
    TEST_ASSERT_TRUE_MESSAGE(Mef_desregistraObservador(&aux,Mef_obtObservador(&dut)),"Debe poder desregistrar un observador registrado");
    Mef_ejecuta(&aux);
    Mef_ejecuta(&dut);
    TEST_ASSERT_EQUAL_UINT_MESSAGE(0,prueba.a.b,"El observador desregistrado no debe recibir el evento B");
    TEST_ASSERT_FALSE_MESSAGE(Mef_desregistraObservador(&aux,Mef_obtObservador(&dut)),"No puede desregistrar un observador no registrado");
}

void test_mefAgotaColaDeEventos(void)
{
    Mef_ejecuta(&dut);
    for(unsigned i=0;i<MAX_EVENTOS;++i){
        TEST_ASSERT_TRUE_MESSAGE(Mef_recibeEvento(&dut,&eventos.evtB),"Debe poder recibir eventos mientras tenga espacio en cola");
    }

    TEST_ASSERT_FALSE_MESSAGE(Mef_recibeEvento(&dut,&eventos.evtB),"No puede recibir eventos una vez agotado el espacio");
}

void test_mefAgotaObservadores(void)
{
    Mef_ejecuta(&dut);
    static ObservadorEventos obs[MAX_OBSERVADORES];
    static ObservadorEventos adicional;

    for(unsigned i=0;i<MAX_OBSERVADORES;++i){
        TEST_ASSERT_TRUE_MESSAGE(Mef_registraObservador(&dut,obs+i),"Debe poder registrar observadores mientras tenga espacio");
    }
    TEST_ASSERT_FALSE_MESSAGE(Mef_registraObservador(&dut,&adicional),"No puede registrar observadores una vez agotado el espacio");
    TEST_ASSERT_FALSE_MESSAGE(Mef_desregistraObservador(&dut,&adicional),"No debe poder desregistrar observador no registrado");
    for(unsigned i=0;i<MAX_OBSERVADORES;++i){
        TEST_ASSERT_TRUE_MESSAGE(Mef_desregistraObservador(&dut,obs+i),"Debe poder desregistrar observadores registrados");
    }
}

void test_mefComoFuenteEventosObservadorNoPuedeRecibir(void)
{
    Mef aux;
    Mef_init(&aux,x);
    TEST_ASSERT_TRUE_MESSAGE(Mef_registraObservador(&aux,Mef_obtObservador(&dut)),"Debe poder registrar un observador de eventos");
    Mef_ejecuta(&dut);
    for(unsigned i=0;i<MAX_EVENTOS;++i)TEST_ASSERT_TRUE(Mef_enviaEvento(&aux,&eventos.evtB));
    TEST_ASSERT_FALSE_MESSAGE(Mef_enviaEvento(&aux,&eventos.evtB),"No debe poder enviar evento si los observadores no pueden recibirlo");
}
