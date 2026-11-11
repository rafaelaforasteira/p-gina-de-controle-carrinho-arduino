# Como explicar o carrinho

## Tecnologias e arquivos

- `index.html`: estrutura da página, textos e elementos SVG (ícones vetoriais).
- `styles.css`: cores, textura, anéis, pulsação e adaptação horizontal/vertical.
- `app.js`: toque, direção, comandos HTTP e botão de tela cheia. JavaScript puro, sem React.
- `manifest.webmanifest`: nome, cores e modo de abertura como aplicativo; não envia comandos e não implementa modo offline.
- `firmware/carrinho/carrinho.ino`: C++ com as bibliotecas WiFi e WebServer do ESP32; recebe pedidos e controla os pinos.
- `scripts/embed_web.py`: Python executado no computador para copiar a página para o firmware.
- `firmware/carrinho/web_assets.h`: cópia gerada automaticamente; estudar e editar os originais, não este arquivo.

Google Fonts fornece as fontes; na falta de internet, o navegador usa as fontes alternativas. Gradientes e SVG desenham o painel sem biblioteca gráfica JavaScript.

## Explicação principal para a aula

“O celular abre a página servida pelo ESP32 na mesma rede Wi-Fi. Quando arrasto o joystick, o JavaScript calcula a posição em relação ao centro e escolhe uma das quatro direções. Se a direção mudou, envia um pedido HTTP. O ESP32 lê o comando e chama a função que altera as entradas da ponte H. Ao soltar, a página centraliza o joystick e tenta enviar parar.”

## Joystick, passo a passo

1. `pointerdown` começa o toque e guarda seu identificador, para não confundir dois dedos.
2. `movePointer` subtrai o centro do joystick da posição do dedo. O resultado é `dx` e `dy`.
3. `moveKnob` calcula a distância com Pitágoras. Se ultrapassar o raio permitido, reduz o vetor para manter o desenho dentro do círculo.
4. Dentro de 20% do deslocamento permitido, escolhe `parar` (zona morta).
5. Fora dela, compara os valores absolutos: se o eixo horizontal domina, escolhe esquerda/direita; senão, frente/ré. Em empate, vale o eixo vertical. Não existem diagonais.
6. `setDirection` atualiza texto e iluminação e só chama `sendCommand` quando a direção muda.
7. `pointerup`, cancelamento e perda de captura chamam `stop`, que centraliza o desenho e muda a direção para parar.

Exemplo: dedo 40 pixels à direita e 10 acima do centro resulta em `dx = 40`, `dy = -10`. Como 40 é maior que 10 e o deslocamento está fora da zona morta, a direção é direita. O sinal negativo de `dy` significa para cima, porque a coordenada vertical da tela cresce para baixo.

## Como a mensagem chega ao motor

Exemplo de endereço: `http://IP_DO_ESP32/comando?acao=frente`.

- `fetch` faz uma requisição HTTP GET.
- `/comando` é a rota; `acao` é o parâmetro; `frente` é seu valor.
- `servidor.on` associa a rota à função `receberComando`.
- `servidor.arg("acao")` lê o valor.
- O `if/else` chama `frente`, `re`, `virarDireita`, `virarEsquerda` ou `parar`.
- Essas funções usam `digitalWrite` para colocar HIGH/LOW nas entradas da ponte H, conforme a ligação testada.
- O servidor responde HTTP 200 com texto `OK`: confirma o atendimento, não mede o movimento físico.

`async/await` espera pela resposta sem bloquear a interface. Existe no máximo um comando em trânsito e um pendente, sempre o mais recente. Isso evita uma fila de movimentos antigos. O timeout limita a espera; não garante que um pedido já enviado deixou de ser executado.

## Conexão, sensor e limites atuais

`/status` responde um JSON simples para verificar acesso ao servidor. Ainda não retorna luminosidade ou velocidade. O indicador pulsa verde se a consulta funciona e vermelho se falha.

O ESP32 lê o LDR com `analogRead`. Abaixo de 300, liga os componentes RGB para formar branco; caso contrário, desliga. Esse número é uma leitura analógica, não lux. Os motores usam HIGH/LOW, sem controle PWM de velocidade.

O programa tenta parar ao soltar, perder foco ou ocorrer erro de comunicação. Porém, se a rede falhar e o pedido não chegar, os motores podem continuar ligados. O firmware para quando o próprio ESP32 perde o Wi-Fi, mas não tem parada por tempo sem comandos. Esta revisão não altera esse comportamento já testado.

## Onde editar e como atualizar

Edite os arquivos originais da raiz. Rode `python scripts/embed_web.py` para regenerar `web_assets.h` e regrave o firmware para atualizar a placa. Atualizar o GitHub não atualiza automaticamente o ESP32. A senha do Wi-Fi deve ficar apenas na cópia local usada para gravação.
