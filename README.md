# Projeto 2 - Sistemas Distribuídos

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

### Linux

Para instalar o protoc-c que permite compilar os ficheiros proto e também para ter acesso aos ficheiros header deve-se correr o seguinte comando para instalar as bibliotecas necessárias:
`sudo apt install libprotobuf-c-dev protobuf-c-compiler`

### MacOS 

De forma a ter acesso ao comando protoc-c e os ficheiros headers necessários para usar o protobuf-c deve-se percorrer o seguinte comando, assumindo que o Homebrew já está instalado: `brew install protobuf-c`

Em sistemas operativos MacOS, deve ser usado o comando `make mac` para gerar os executáveis.

---

### Trabalho realizado por:

**Grupo 8**

Alexandre Pinto - 55958

Eduardo Proença - 57551

Tiago Oliveira - 54979

