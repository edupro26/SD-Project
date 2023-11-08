# Projeto 3 - Sistemas Distribuídos

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

### Desenvolvimento
- Foram desenvolvidas as funções read_all e write_all de forma a garantir a leitura e escrita de todos os bytes pretendidos.
- É chamada a função `signal` para ignorar o sinal SIGPIPE, de forma a que o programa não termine caso o cliente feche a ligação.
- Usamos a função `setsockopt` para permitir que o servidor reutilize o endereço e porta do socket, de forma a que o servidor possa ser reiniciado sem ter que esperar que o socket seja libertado.
- Em caso de erro em qualquer uma das partes do código do servidor durante o processamento de uma mensagem, é enviado a mensagem ao cliente e o servidor continua a correr.
- Implementamos a conexão ao servidor através de hostname (nome do servidor) e porta, além de apenas através do endereço IPv4.

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

---

### Valgrind

Comandos para executar o Valgrind:

`valgrind --leak-check=yes --show-leak-kind=all ./binary/table_server <port> 10`

`valgrind --leak-check=yes --show-leak-kind=all  ./binary/table_client <ip>:<port>`

### Trabalho realizado por:

**Grupo 8**

Alexandre Pinto - 55958

Eduardo Proença - 57551

Tiago Oliveira - 54979

