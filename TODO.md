
# TODO

# Client
O Client tem que passar a conectar-se ao zk para saber a que server se tem que connectar. Um server para escrita outro para leitura
O Server tem que conectar-se ao zookeeper para dar conhecimento ao zookeeper da sua existencia


- Atualizar o stats
- O que fazer em caso de falha a meio da chain


# Messages
## PUT
### Success
11
CT_NONE
### Already exists key and value
11
CT_NONE
### Already exists key 
11
CT_NONE
## Get
### Success
21
CT_VALUE
### Doesnt exist
OP_ERROR
CT_NONE
## Del
### Success
31
CT_NONE
### Doest exist
OP_ERROR
CT_NONE
## Getkeys
### Success
51
CT_Keys
### Empty
51
CT_Keys
## Gettable
### Success
61
CT_TABLE
### Empty
61
CT_TABLE

