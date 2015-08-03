# New repo for qtxmllib: https://bitbucket.org/alexSlyshyk/qtxmlrpc #

Simple qt 4.x/5.x xmlrpc server & client classes, ssl support.

You can register your slots as xmlrpc callbacks in your qt app.

```
XmlRpcServer * rpcServer = new XmlRpcServer();
rpcServer->listen( QHostAddress("127.0.0.1"), 5430 );
rpcServer->registerSlot( rpcServer, SLOT(echo(const QVariant&)) );
rpcServer->registerSlot( rpcServer, SLOT(deferredEcho(const QVariant&)) );
```
```
public slots:
      QVariant echo( const QVariant& e ) { return e; }
      DeferredResult * deferredEcho( const QVariant& e ) { return new DeferredEcho( e ); }
```

Compile sources, then:
```
$ ./echo
23:58:20.146 [0x618550 WRN] listening http://127.0.0.1:5430/RPC2
```
Another console:
```
$ python
Python 2.5.1 (r251:54863, Nov  4 2007, 17:58:10)
[GCC 4.1.2 (Gentoo 4.1.2)] on linux2
Type "help", "copyright", "credits" or "license" for more information.

>>> import xmlrpclib
>>> server = xmlrpclib.ServerProxy("http://localhost:5430")
>>> server.echo( [1,2,{'a':1, 'b':2}] )
[1, 2, {'a': 1, 'b': 2}]
>>> server.deferredEcho( [1,2,{'a':1, 'b':2}] )
[1, 2, {'a': 1, 'b': 2}]
```

Deffered result can be emmited as signal, from derived class. So you can do sql, network, thread and so on work in your callbacks.

S.Y.
Kirill Kostuchenko : ddosoff@gmail.com

&lt;wiki:gadget url="http://www.ohloh.net/p/117248/widgets/project\_thin\_badge.xml" height="52" border="0"/&gt;