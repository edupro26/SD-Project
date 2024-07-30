# Projeto 4 - Sistemas Distribuídos

### Distributed Systems Project

Teacher: António Casimiro Ferreira da Costa

Grade: 20/20

---

Trabalho desenvolvido para a disciplina de Sistemas Distribuídos da Faculdade de Ciências da Universidade de Lisboa.

## Instruções de utilização
É incluido um ficheiro Makefile que deve ser utilizado para compilar o programa. Para isso basta correr o comando `make` na pasta onde se encontra o ficheiro Makefile. São criados 2 executáveis, um para o servidor e outro para o cliente.

O diretório do projeto é consituído pelos seguintes subdiretórios:
- `source` - contém os ficheiros fonte do projeto
- `include` - contém os ficheiros de cabeçalho do projeto
- `dependencies` - contém os ficheiros de dependências do projeto
- `objects` - contém os ficheiros objeto do projeto
- `lib` - contém os ficheiros de bibliotecas usados no projeto
- `binary` - contém os executáveis do projeto

De forma a eliminar os ficheiros gerados pela compilação, basta correr o comando `make clean` na pasta root do projeto. 

#### Iniciar e parar o ZooKeeper 

- $ zkServer.sh start
- $ zkServer.sh stop

#### Iniciar Servidor

- $ cd /binary
- $ ./table_server <port> <n_lists> <zk_ip:port>

#### Conectar Cliente

- $ cd ./binary
- $ ./table_client <zk_ip:port>


### Desenvolvimento

#### Fase 2

- Foram desenvolvidas as funções read_all e write_all de forma a garantir a leitura e escrita de todos os bytes pretendidos.
- É chamada a função `signal` para ignorar o sinal SIGPIPE, de forma a que o programa não termine caso o cliente feche a ligação.
- Usamos a função `setsockopt` para permitir que o servidor reutilize o endereço e porta do socket, de forma a que o servidor possa ser reiniciado sem ter que esperar que o socket seja libertado.
- Em caso de erro em qualquer uma das partes do código do servidor durante o processamento de uma mensagem, é enviado a mensagem ao cliente e o servidor continua a correr.
- Implementamos a conexão ao servidor através de hostname (nome do servidor) e porta, além de apenas através do endereço IPv4.

#### Fase 3

Nesta fase, a conexão de vários cliente é possível através do use de threads com um thread para cada cliente. Para isso, foi criada a função `handle_client` que contêm grande parte da lógica que estava presente na função `network_main_loop` que recebe e envia mensagens do cliente conectado. Assim, a função `network_main_loop` apenas fica responsável por aceitar novas conexões e criar um thread para cada cliente que se conecta ao servidor.

Foi criada uma nova operação executada com o comanda `stats` que permite obter o número de clientes conectados ao servidor, numero de operações realizada na tabela (leitura e escrita) e o tempo total consumido a realizar essas operações. Para isso foi criada a estrutura `statistics_t`. Os valores são atualizados nas funções do módulo `network_server` à medida que mensagens são recebidas e enviadas bem como quando um cliente se conecta ou desconecta do servidor.

Com o uso de multiplas threads, a concorrência é um problema que pode ocorrer. Para resolver este problema usamos mutexes para garantir que apenas uma thread pode escrever na tabela de cada vez e também para garantir que apenas uma thread pode escrever nas estatísticas de cada vez. No caso de leitura, se estiver a ser feita uma escrita, a leitura é bloqueada até que a escrita termine. 
Para implementar os mutexes, foi criado o modulo `locks` que contêm definida a estrutura `locks_t` que conêm os mutexes, os contadores, e as variáveis de condição para cada um dos mutexes. A estrutura é inicializada na função `locks_init` e é libertada a memória na função `destroy_lock`. Os contadores guardam quantas threads estão a ler, escrever ou à espera de escrever. As variáveis de condição são usadas para bloquear as threads que estão à espera de escrever ou ler.
É incializado uma estrutura locks_t para escritas/leituras na tabela e outra para escritas/leituras nas estatísticas.

Foi ainda implementado uma função extra que termina o servidor ao receber signais do tipo SIGINT. Desta forma, o servidor ao ser terminado via CTRL+C, este termina de forma correta, libertando a memória alocada e fechando o socket. Para conseguir que a memória alocado fosse toda libertada, mesmo quando ainda existem clientes ligados, foi necessário a criação de uma estrutura de nós ligados que contêm o socket de cada cliente conectado e a respectiva thread que está a tratar desse cliente. Quando é feito o pedido para terminar o servidor, é chamada, para cada thread, a função `pthread_cancel` para ordenard a thread a fechar, seguida da função `pthread_join` para esperar que a thread termine. Depois de todas as threads terminarem, é fechado o socket do servidor e libertada a memória alocada. Para isto, não fazemos uso da função `pthread_detach` de forma a podermos esperar que as threads terminem e a memória alocada seja libertada.

### Fase 4

Nesta fase foi implementado o Zookeeper. Desta forma, podemos ter vários servidores ligados, permitindo que numa eventual falha de um servidor, os clientes continuem a poder executar operações. Foram criados os ficheiros `zk_client.h` e `zk_server.h` que descrevem as várias funções que foram implementadas nesta fase do projeto, funções essas que estão presentes nos ficheiros `zk_client.c` e `zk_server.c`.

### Entrega

O arquivo de entrega contêm todos os ficheiros source e headers necessários para a compilação do programa, bem como os quatros ficheiros objeto providenciados no moodle para gerar a biblioteca libtable.a. Também é providenciado um ficheiro Makefile que permite compilar o programa com os seguintes commandos:

- `make` - Compila o programa por completo, gerando a biblioteca libtable.a e os executáveis do servidor e cliente.
- `make libtable.a` - Compila a biblioteca libtable.a.
- `make table-server` - Gera o executável do servidor. Assume que a biblioteca libtable.a já foi compilada.
- `make table-client` - Gera o executável do cliente. Assume que a biblioteca libtable.a já foi compilada.

Ao utilizar o `make` são criadas as pastas necessárias para os ficheiros gerados caso estas não existam.

### Execução em linux

Para instalar o protoc-c que permite compilar o ficheiro proto e também para ter acesso aos ficheiros header deve-se correr o seguinte comando para instalar as bibliotecas necessárias:
`sudo apt install libprotobuf-c-dev protobuf-c-compiler`

### MacOS 

De forma a ter acesso ao comando protoc-c e os ficheiros headers necessários para usar o protobuf-c deve-se percorrer o seguinte comando, assumindo que o Homebrew já está instalado: `brew install protobuf-c`

Em sistemas operativos MacOS, deve ser usado o comando `make mac` para gerar os executáveis.

### Valgrind

Comandos para executar o Valgrind:

`valgrind -s --leak-check=full --show-leak-kinds=all ./binary/table_server <port> <n_lists>`

`valgrind -s --leak-check=full --show-leak-kinds=all  ./binary/table_client <ip>:<port>`

---

### Trabalho realizado por:

**Grupo 8**

Alexandre Pinto - 55958

Eduardo Proença - 57551

Tiago Oliveira - 54979

