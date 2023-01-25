
#include <WiFi.h>
//definção das váriaveis do id e senha da rede
const char* ssid     = "id da rede";
const char* senha = "senha da rede";

WiFiServer server(80); //definição da porta do servidor

void setup()
{
    Serial.begin(115200);
    pinMode(2, OUTPUT);      // Setar qual vai ser o PINO de saída, nesse caso o led do próprio ESP32 

    delay(10);

    // Conectando à rede WIFI

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, senha); // inicia a rede WIFI

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); //Print do endereço IP da ESP
    
    server.begin(); //inicio do servidor

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // Espera por clientes

  if (client) {                             // Se um cliente fazer um request,
    Serial.println("Um cliente entrou.");   // Realizar um print avisando a situação
    String currentLine = "";                // é necessário uma string para segurar a falta da chegada de dados 
    while (client.connected()) {            // Realizar um loop enquanto o cliente está conectando 
      if (client.available()) {             // Se tiver bytes para ler do cliente, então
        char c = client.read();             // Leia os bytes, então
        Serial.write(c);                    // printar
        if (c == '\n') {                  

        
          // Pronto, terminou o processo de request http do cliente, então mande a resposta:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            //Conteúdo da resposta HTTP, ou seja, o código HTML 
                client.println("<html>");
                client.println("<head>");
                client.println("<meta charset=\"UTF-8\">");
                client.println("<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">");
                client.println("<meta name=\"viewport""content=\"width=device-width, initial-scale=1.0\">");
                client.println("<style>");
                client.println("body{ margin: auto;}");
                client.println("header{ margin: auto; background-color: aquamarine; }");
                client.println("img{ padding-left: 25px;}");
                client.println(".botaoliga{background-color:#00FF00;color:white;padding:15px 40px;border-radius:25px;}");
                client.println(".botaodesliga{background-color:#FF0000;color:white;padding:15px 40px;border-radius:25px;}");
                client.println(".botaodesliga:hover{opacity: 70%;}");
                client.println(".botaoliga:hover{ opacity: 70%;}");
                client.println("#inicio{ text-decoration: none; padding-left: 25px; color: rgb(90, 90, 90);") ;
                client.println("font-family: 'source sans pro', sans-serif;}");
                client.println("#inicio:hover{ opacity: 80%;}");
                client.println("ul{ display: inline; margin-bottom: 0px;}");
                client.println("nav{ padding-bottom: 20px;}");
                client.println(".botao{ padding-left: 20px; text-align: center;}");
                client.println("</style>");
                client.println("<title>SUAP</title>");
                client.println("</head>");
                client.println("<body>");
                client.println("<header>");
                client.println("<nav><ul>");
                client.println("<li><a id=\"inicio\" href=\"https://suap.ifrn.edu.br/\">Início</a>");
                client.println("</li>");
                client.println("</ul></nav>");
                client.println("</header><main>");
                client.println("<p class=\"botao\"><a href=\"/H\"><button class=\"botaoliga\">LIGA</button></a></p>");
                client.println("<p class=\"botao\" ><a href=\"/L\"><button class=\"botaodesliga\">DESLIGA</button></a></p>");
                client.println("</main></body></html>");

            
            client.println();
            // quebra do loop:
            break;
          } else {    // else para manter o loop até um cliente conectar
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      

        // Comandos que checam se o cliente adicionou um comando, no caso, o /H e /L
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(2, HIGH);               // o comando GET /H ativa o led
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(2, LOW);                // o comando GET /L desliga o led
        }
      }
    }
    // caso o cliente desconecte, é encerrada a conexão
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
