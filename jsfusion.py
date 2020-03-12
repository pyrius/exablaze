from jsonrpc_requests import Server

fusion = Server("http://fusion1/jsonrpc")
fusion.login(username="admin", password="admin")
#print (fusion.get_hostname())
#print (fusion.get_startup_config("patch"))
#print (fusion.get_messages())
pl = fusion.get_ports("A1","A2","A3")
print (pl[0])

for i in pl:
    for k in i.keys():
        if (i[k]!=''):
            if (k != 'name'):
             print("   {:<16s} : {:2s} ".format(k,str(i[k])))
            else:
                print (i[k])
    print ('\n')
