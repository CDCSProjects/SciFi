import duckstore_wrapper


duck = duckstore_wrapper.PyDuckStore("galaxymeta".encode('utf-8'))

duck.createNewDB()
duck.execQueryAndPrint("CREATE IF NOT EXISTS TABLE newtab(id INTEGER)".encode('utf-8'))
duck.execQueryAndPrint("Select distinct split_CDL FROM metadata".encode('utf-8'))
duck.getSingleToFile("FRII/3.845_0.156_1_MiraBest.png".encode('utf-8'), ".txt".encode('utf-8'))