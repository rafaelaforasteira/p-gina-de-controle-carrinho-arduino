# Carrinho IOT

Para estudar o código e preparar a explicação em aula, leia [o roteiro de apresentação](APRESENTACAO.md). Os arquivos originais agora têm comentários em português; a decisão de direção do joystick usa `if/else`, mantendo a lógica anterior.

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

A página envia `GET /comando?acao=...`, com as ações frente, re, direita, esquerda e parar. Envia ao mudar a direção e tenta enviar parar ao soltar, perder foco ou ocultar a página. Mantém apenas o comando pendente mais recente.

O firmware usa funções simples, if/else e delay(50), sem millis(). Não há parada por tempo sem comandos. Se apenas o celular perder comunicação e o comando parar não chegar, os motores podem continuar em movimento. A perda do Wi-Fi do próprio carrinho provoca parada. Faça testes com as rodas suspensas e use apenas um controle por vez.

A rota /status responde para o indicador de conexão. Os pinos da ponte H são 25, 26, 27 e 32; os LEDs RGB e o LDR no pino 34 seguem o código original, com limite de luz 300. A leitura do sensor ocorre no loop. O teste que movimentava o carrinho automaticamente ao ligar foi removido.

## Atualizar o painel embutido

Depois de editar os arquivos web da raiz, execute:

```sh
python3 scripts/embed_web.py
```

Grave novamente o firmware para atualizar o painel no ESP32. O GitHub Pages e o firmware usam a mesma versão de JavaScript, mas o Pages mantém como destino alternativo `http://192.168.10.108`; CORS, HTTPS e permissões de rede local podem impedir seu uso.

## Interface

Joystick por mouse, toque ou setas do teclado quando focado. Solte para parar. Botão de tela cheia nos navegadores compatíveis. No iPhone, o modo de tela inicial depende do Safari. Fontes externas têm fallback local; não é necessário internet para os comandos quando o painel é servido pelo ESP32.

## Diagnóstico de acesso

O último IP informado pelo Monitor Serial foi `192.168.10.108`, mas o roteador pode atribuir outro endereço depois. O painel servido pelo ESP32 usa o IP da própria página.

Se o navegador mostrar ERR_CONNECTION_TIMED_OUT, teste no Chrome/Safari de um celular ou PC conectado à rede iot2. Um navegador remoto ou integrado pode não ter acesso à rede local. Tente primeiro `http://192.168.10.108/status`, sem enviar comandos de motor. Se não responder, confira o IP atual no Monitor Serial, a alimentação e a conexão à rede. Redes de convidados ou com isolamento entre aparelhos também podem impedir o acesso.

Atualizar o GitHub não altera o código já gravado na placa. Para usar novos arquivos embutidos, baixe carrinho.ino e web_assets.h, preencha a senha localmente e grave novamente.

## Validação

A sintaxe JavaScript e o envio das cinco ações foram verificados com HTTP simulado. Também foram conferidos o IP local 192.168.10.108, o envio de parar e a igualdade entre os arquivos web e suas cópias no header.

Não houve compilação com o core Arduino ESP32 nem gravação ou teste físico neste ambiente. O log enviado pelo usuário confirma que sua placa conectou ao Wi-Fi e iniciou o servidor; o acesso ao painel e os motores ainda precisam de teste local.
