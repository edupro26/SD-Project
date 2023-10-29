
# TODO

- Fix invalid reads on commands get and gettable (see valgrind output)
- Fix memory leaks on the server part
- Catch signal
- Makefile observer headers changes


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

