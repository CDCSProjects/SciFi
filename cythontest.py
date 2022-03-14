#!/usr/bin/env python3
import assetstore

duck = assetstore.PyDuckStore("galaxymeta".encode('utf-8'))
duck.execQueryAndPrint("Select distinct split_CDL FROM metadata".encode('utf-8'))
duck.getSingle("FRII/3.845_0.156_1_MiraBest.png".encode('utf-8'))

rocks =assetstore.PyRocksStore("galaxyasset".encode('utf-8'))
rocks.open();
rocks.getSingleToFile("FRII/3.198_0.788_1_Capetti2017b.png".encode('utf-8'), "".encode('utf-8'))
