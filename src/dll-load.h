typedef struct
{
    const char	*name;
    void	*func;

} dllimport_t;

typedef struct
{
    unsigned int	name;
    void		*func;

} nvapiimport_t;

HMODULE *dynaload_module(const char *name,    dllimport_t *table);
HMODULE *nvapiload_module(const char *name, nvapiimport_t *table);

void  dynaload_close(void *handle);


