# Carrinho IOT

Painel HTML/CSS/JavaScript e firmware ESP32 integrados. O controle físico original continua compatível com `GET /comando?acao=frente|re|direita|esquerda|parar`.

## Gravar e abrir o painel

1. Baixe o repositório e mantenha juntos os arquivos da pasta `firmware/carrinho`.
2. Abra `firmware/carrinho/carrinho.ino` na Arduino IDE com o suporte ESP32 instalado.
3. Preencha `senha` com a senha do Wi-Fi na sua cópia local. A rede configurada é `iot2`. Não publique credenciais no GitHub.
4. Selecione sua placa ESP32 e porta USB e grave o sketch. O arquivo `web_assets.h` já contém o painel; não exige upload separado para um sistema de arquivos.
5. Abra o Monitor Serial em 9600 baud e veja o endereço IP atribuído ao carrinho.
6. Conecte o celular/PC à mesma rede e abra `http://IP_EXIBIDO/`. Não use o GitHub Pages para dirigir o carrinho: HTTPS para um destino HTTP pode ser bloqueado pelo navegador.

O painel servido pelo próprio ESP32 usa a mesma origem e acompanha o IP atribuído pelo roteador. O IP não é fixado pelo firmware. O controle físico original ainda precisa ter seu `ipServidor` ajustado caso o IP mude.

## Comandos e parada

A página envia `GET /comando?acao=...&origem=web`. Enquanto há movimento, renova o comando a cada 250 ms. Ao soltar, perder foco ou ocultar a página, tenta enviar `parar`. O firmware para movimentos iniciados pela página após 1200 ms sem renovação e também para ao perder o Wi-Fi. A fila da página mantém só o comando pendente mais recente.

O timeout de renovação se aplica aos comandos com `origem=web`. O controle físico enviado pelo usuário só transmite ao apertar/soltar e mantém esse comportamento; não possui a mesma proteção contra perda de seu comando de soltar. Use apenas um controle por vez.

O firmware atende `/status` com HTTP 200 para o indicador de conexão. Mantém os pinos da ponte H (25, 26, 27, 32), os dois LEDs RGB e o LDR no pino 34, com limite 300. A leitura de luz ocorre a cada 250 ms sem bloquear o atendimento HTTP. O teste automático que movimentava o carrinho ao ligar foi removido. As rotinas de pisca que só eram usadas nesse teste não são executadas.

## Atualizar o painel embutido

Depois de editar os arquivos web da raiz, execute:

```sh
python3 scripts/embed_web.py
```

Grave novamente o firmware para atualizar o painel no ESP32. O GitHub Pages e o firmware usam a mesma versão de JavaScript, mas o Pages mantém como destino alternativo `http://192.168.10.103`; CORS, HTTPS e permissões de rede local podem impedir seu uso.

## Interface

Joystick por mouse, toque ou setas do teclado quando focado. Solte para parar. Botão de tela cheia nos navegadores compatíveis. No iPhone, o modo de tela inicial depende do Safari. Fontes externas têm fallback local; não é necessário internet para os comandos quando o painel é servido pelo ESP32.

## Validação

Sintaxe JavaScript e testes com HTTP simulado verificaram as cinco ações, renovação de movimento, parada ao soltar, endereço IP dinâmico e descarte de comandos antigos na fila. Os arquivos embutidos foram regenerados pelo script.

Não houve compilação com o core Arduino ESP32 nem gravação/teste de hardware neste ambiente. A primeira validação no hardware deve ser feita com as rodas suspensas.
