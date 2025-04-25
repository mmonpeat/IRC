#Bon diaaa

### Architecture
~~~
class   Server
{
    public:
    //constructors and destructors

        getServerPort();
        getServerPass();

        checkNick(client);
        checkPowers(client, channel);
    private:
        int     server_port;
        int     server_pass;
        int     server_socket_fd;
        vector  clients;
        vector  channels;

        ft_lookup() //looks for clients in struct;
};

class   Client
{
    public:
        //constructors and destructors

    private:
        int     password;
        int     fd;
        string  nick;
        string  user;
        string  real_name;
        list of channels belong to? maybe maybe not
};

class   Channel
{
    //constructors destructors
    public:
    private:
        string      name;
        string[]    client_nicks;
        vector      operators;
        vector      clients;        //if a client becomes an operator, it's removed from clients and added to operators.
};
~~~
Numerical_replies.h //in which all num replies are defined

### Subject Requirements vs IRC protocols

#### Commands

PASS
NICK
USER

After registration:
JOIN
