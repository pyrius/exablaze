from jsonrpc_requests import Server

<<<<<<< HEAD:js.py
<<<<<<< HEAD:js.py
<<<<<<< HEAD:js.py
fusion = Server("http://EMEA-DCRS-FM-48X-1/jsonrpc")
fusion.login(username="admin", password="cisco")
=======
fusion = Server("http://fusion1/jsonrpc")
fusion.login(username="admin", password="admin")
>>>>>>> parent of 63ba80f (emea-calo change):jsfusion.py
=======
fusion = Server("http://fusion1/jsonrpc")
fusion.login(username="admin", password="admin")
>>>>>>> parent of 63ba80f (emea-calo change):jsfusion.py
=======
fusion = Server("http://fusion1/jsonrpc")
fusion.login(username="admin", password="admin")
>>>>>>> parent of 63ba80f (emea-calo change):jsfusion.py
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
