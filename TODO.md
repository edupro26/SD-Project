
# TODO

- Fix invalid reads on commands get and gettable (see valgrind output)
- Fix memory leaks on the server part
- Catch signal
- Makefile observer headers changes
- Implement connection by hostname
- Caso algum dos pedidos não possa ser atendido devido a um erro, o servidor vai retornar {OP_ERROR, CT_NONE} independentemente do tipo de erro.
