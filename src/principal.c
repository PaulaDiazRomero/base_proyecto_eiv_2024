#include "gpio.h"
#include "tempo_ms.h"
#include "serie.h"
#include "teclado.h"
#include "es_lcd.h"
#include "controlador.h"
#include "container_of.h"
#include "mef.h"
#include "pantalla.h"
#include "receptor.h"

static void seleccionaFilaTeclado(unsigned fila);
static unsigned leeColumnasTeclado(void);


int main(void)
{
    static uint32_t tiempoInicial;
    static EventoInt caracterRecibido={.evento.mensaje=MensajeInt_CARACTER_RECIBIDO};
    static Pantalla pantalla;
    static const EventoInt eventosTecla[]={
        [CodigoTecla_0]={.evento.mensaje=MensajeInt_DIGITO,.valor=0}, //"Tecla 0 presionada! \n",
        [CodigoTecla_1]={.evento.mensaje=MensajeInt_DIGITO,.valor=1},//"Tecla 1 presionada! \n",
        [CodigoTecla_2]={.evento.mensaje=MensajeInt_DIGITO,.valor=2},//"Tecla 2 presionada! \n",
        [CodigoTecla_3]={.evento.mensaje=MensajeInt_DIGITO,.valor=3},//"Tecla 3 presionada! \n",
        [CodigoTecla_4]={.evento.mensaje=MensajeInt_DIGITO,.valor=4},//"Tecla 4 presionada! \n",
        [CodigoTecla_5]={.evento.mensaje=MensajeInt_DIGITO,.valor=5},//"Tecla 5 presionada! \n",
        [CodigoTecla_6]={.evento.mensaje=MensajeInt_DIGITO,.valor=6},//"Tecla 6 presionada! \n",
        [CodigoTecla_7]={.evento.mensaje=MensajeInt_DIGITO,.valor=7},//"Tecla 7 presionada! \n",
        [CodigoTecla_8]={.evento.mensaje=MensajeInt_DIGITO,.valor=8},//"Tecla 8 presionada! \n",
        [CodigoTecla_9]={.evento.mensaje=MensajeInt_DIGITO,.valor=9},//"Tecla 9 presionada! \n",
        [CodigoTecla_ASTERISCO]={.evento.mensaje=Mensaje_ASTERISCO},//"Tecla * presionada! \n",
        [CodigoTecla_NUMERAL]  ={.evento.mensaje=Mensaje_NUMERAL},//"Tecla # presionada! \n",
        [CodigoTecla_NO_VALIDO]={.evento.mensaje=Mensaje_TECLA_NO_VALIDA},//"Error al leer tecla! \n",
    };
    Mef *const controlador = Controlador_init(180,24*60);
    Mef *const receptor = Receptor_init();

    Teclado teclado;
    Tempo_inicializa();
    Lcd *const miLcd = inicializaLcd();
    Pantalla_init(&pantalla,miLcd);
    Mef_registraObservador(receptor,Mef_obtObservador(controlador));
    Mef_registraObservador(controlador,Pantalla_obtObservador(&pantalla));
    Serie_init(9600);
    Pin_configuraSalida(PIN_LED,PUSH_PULL,V_BAJA);
    Pin_ponAlto(PIN_LED);
    Teclado_inicializa(&teclado,&seleccionaFilaTeclado,&leeColumnasTeclado);
    Lcd_limpiaPantalla(miLcd);
    Lcd_mueveAInicio(miLcd);
    Lcd_escribeCadena(miLcd,"*Iniciar ciclo");
    Lcd_establecePosicion(miLcd,1,0);
    Lcd_escribeCadena(miLcd,"#Configurar");
    tiempoInicial = Tempo_obtMilisegundos();
    for(;;)
    {
        const uint32_t tiempo = Tempo_obtMilisegundos();
        char c;
        if(Serie_intentaRecibirCaracter(&c)){
            caracterRecibido.valor=c;
            Mef_recibeEvento(receptor,&caracterRecibido.evento);
        }        
        if(tiempo-tiempoInicial >= 10){
            Teclado_procesa(&teclado);
            if(Teclado_hayEntradaPendiente(&teclado)){
                CodigoTecla cod = Teclado_obtEntrada(&teclado);
                Pin_ponBajo(PIN_LED);
                Mef_recibeEvento(controlador,&eventosTecla[cod].evento);
            }else{
                Pin_ponAlto(PIN_LED);
            }
            tiempoInicial = tiempo;
        }
        Mef_ejecuta(receptor);
        Mef_ejecuta(controlador);
        Pantalla_ejecuta(&pantalla);
    }
    return 0;
}


static void seleccionaFilaTeclado(unsigned fila)
{
    static Bus filas = INICIALIZA_VARIABLE_BUS(PULL_UP,DRENADOR_ABIERTO,V_BAJA,
    PA5,PA6,PA7,PB0);
    Bus_escribe(&filas,~(0b1000UL >> fila));
}
static unsigned leeColumnasTeclado(void)
{
    static Bus columnas = INICIALIZA_VARIABLE_BUS(PULL_UP,DRENADOR_ABIERTO,V_BAJA,
    PB1,PB10,PB11);
    unsigned valor;
    Bus_lee(&columnas, &valor);
    return valor;
}
