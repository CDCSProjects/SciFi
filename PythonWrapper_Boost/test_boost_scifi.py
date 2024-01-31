import unittest
import boost_scifi
import os
import shutil

class SciFiWrapTest(unittest.TestCase):

    storage = boost_scifi.Storage('assetstore_test', 'metastore_test')
    @classmethod
    def tearDownClass(cls):
        if os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/metastore_test'):
            os.remove('/home/katharina/SciFi/PythonWrapper_Boost/metastore_test')
            os.remove('/home/katharina/SciFi/PythonWrapper_Boost/metastore_test.wal')
        if os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/meta_test'):
            os.remove('/home/katharina/SciFi/PythonWrapper_Boost/meta_test')
            os.remove('/home/katharina/SciFi/PythonWrapper_Boost/meta_test.wal')
        if os.path.isdir('/home/katharina/SciFi/PythonWrapper_Boost/assetstore_test'):
            shutil.rmtree('/home/katharina/SciFi/PythonWrapper_Boost/assetstore_test')
        if os.path.isdir('/home/katharina/SciFi/PythonWrapper_Boost/asset_test'):
            shutil.rmtree('/home/katharina/SciFi/PythonWrapper_Boost/asset_test')
        if os.path.isdir('/home/katharina/SciFi/PythonWrapper_Boost/asset_test2'):
            shutil.rmtree('/home/katharina/SciFi/PythonWrapper_Boost/asset_test2')

    def setUp(self):

        self.__class__.storage.load_metadata_from_file('/home/katharina/SciFi/example_datasets/hamburgimages/usermeta.csv', 'key')
        self.storage.load_assets_from_dir('/home/katharina/SciFi/example_datasets/hamburgimages/assets')

    def tearDown(self):
        for file in os.listdir('/home/katharina/SciFi/PythonWrapper_Boost/results'):
            if os.path.isfile(os.path.join('/home/katharina/SciFi/PythonWrapper_Boost/results/', file)):
                os.remove(os.path.join('/home/katharina/SciFi/PythonWrapper_Boost/results/', file))

    def test_load_asset_from_dir(self):
        #TODO momentan keine Fehlermeldung wenn bspw. Pfad nicht korrekt, überprüfen ob input funktioniert
        self.__class__.storage.load_assets_from_dir('/home/katharina/SciFi/example_datasets/hamburgimages/assets')
        with self.assertRaises(RuntimeError):
            self.__class__.storage.load_assets_from_dir('home/katharina/SciFi/example_datasets/')

    def test_load_metadata_from_file(self):
        #TODO same as above, check if successful difficult because no assertion is thrown by incorrect path
        path_csv = '/home/katharina/SciFi/example_datasets/hamburgimages/usermeta.csv'
        self.__class__.storage.load_metadata_from_file('/home/katharina/SciFi/example_datasets/hamburgimages/usermeta.csv', 'key')
        self.__class__.storage.load_metadata_from_file('home/katharina/SciFi/example_dataset/hamburgimages/usermeta.csv', 'key')
    def test_get_single_id(self):
        #test return string
        result = self.__class__.storage.get('wikipedia')
        with open('/home/katharina/SciFi/example_datasets/hamburgimages/assets/wikipedia.txt') as file:
            expected = file.read().replace('\n', '\r\n')
            self.assertEqual(expected, result)

    def test_get_single_id_to_file(self):
        #test return to file for single id
        self.__class__.storage.get('wikipedia', assetToFile=1, fileextension='.svg', metaToFile=1, fileextension_meta='.txt')
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/wikipedia.svg'))
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/wikipediameta.txt'))

    def test_get_multiple_ids(self):
        #test return string for single id
        #TODO same as above, rückgabewert überprüfen
        result = self.__class__.storage.get(['flag', 'wikipedia'])
        self.assertEqual(2,len(result))
        result=self.__class__.storage.get(['flags', 'wikipedia'])
        self.assertEqual(result[0],'')

    def test_get_multiple_ids_to_file(self):
        #test return to file for multiple ids
        ids = ['flag', 'wikipedia']
        result_path = '/home/katharina/SciFi/PythonWrapper_Boost/results/'
        fileextension = '.svg'
        self.__class__.storage.get(ids, assetToFile=1, fileextension='.svg', metaToFile=1, fileextension_meta='.txt')
        self.assertTrue(os.path.isfile(result_path+ids[0]+fileextension))
        self.assertTrue(os.path.isfile(result_path+ids[1]+fileextension))
        self.assertTrue(os.path.isfile(result_path+'metadata.txt'))


    def test_get_all_assets_ids_only(self):
        all_ids = ['flag', 'lighthouse', 'ship', 'wikipedia', 'wittenberger_heide']
        result = self.__class__.storage.get_all_assets(ids_only=1)
        for i in range(len(result)):
            self.assertIn(result[i],all_ids)
        self.assertEqual(len(result), len(all_ids))

    def test_get_all_assets_to_file(self):
        all_ids = ['flag', 'lighthouse', 'ship', 'wikipedia', 'wittenberger_heide']

        self.__class__.storage.get_all_assets(assetToFile=1, fileextension='.svg', metaToFile=1, fileextension_meta='.txt')

        for i in range(len(all_ids)):
            file_path = '/home/katharina/SciFi/PythonWrapper_Boost/results/' + all_ids[i] + '.svg'
            self.assertTrue(os.path.isfile(file_path))

        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/metadata.txt'))

    def test_remove_asset(self):
        id = 'ship'
        self.__class__.storage.remove_asset(id, remove_metadata=1)
        current_ids = self.__class__.storage.get_all_assets(ids_only=1)
        self.assertNotIn(id, current_ids)
        self.__class__.storage.get_all_assets(assetToFile=1, fileextension='.svg')
        self.assertFalse(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/ship.svg'))
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/flag.svg'))

    def test_filter(self):

        filter = "priority>20"

        self.assertEqual(['ship','flag'], self.__class__.storage.get_IDs_by_constraint(filter))

        self.__class__.storage.get_by_constraint(filter,assetToFile=1, fileextension='.svg')
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/ship.svg'))
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/flag.svg'))
        self.__class__.storage.store_filter(filter, "prio", "every asset with priority higher 20")
        self.assertIn('prio', self.__class__.storage.get_all_filters())

        #TODO apply filter function does not generate files as output, returns simply ids, auskommentiert
        ids = self.__class__.storage.apply_filter('prio', assetToFile=1, fileextension='.svg')
        for i in range(len(ids)):
            self.assertIn(ids[i],['ship','flag'])

        self.assertEqual('every asset with priority higher 20', self.__class__.storage.get_filter_text('prio'))

        self.__class__.storage.remove_filter('prio')
        self.assertNotIn('prio', self.__class__.storage.get_all_filters())


    def test_create_empty_metadata_table(self):
        expected_meta = ['my_name', 'my_value', 'VARCHAR', 'FLOAT', '[', 'Rows:', '0]']
        self.__class__.storage.create_empty_metadata_table("my_name VARCHAR, my_value FLOAT")
        self.__class__.storage.get_by_constraint("all", metaToFile=1, fileextension_meta=".txt")
        with open("results/metadata.txt") as meta_file:
            meta = meta_file.read()
            meta = meta.split()
            self.assertEqual(meta, expected_meta)


    def test_load_asset_from_file(self):
        self.assertTrue(os.path.isfile("/home/katharina/SciFi/PythonWrapper_Boost/test_asset.svg"))
        self.__class__.storage.load_asset_from_file("panther.txt", "/home/katharina/SciFi/PythonWrapper_Boost")
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", self.__class__.storage.get_all_assets(ids_only=1))
        self.__class__.storage.get("panther", assetToFile=1, fileextension=".txt")
        self.assertTrue(os.path.isfile("/home/katharina/SciFi/PythonWrapper_Boost/results/panther.txt"))


    def test_import_asset_store(self):
        path_to_asset_store='/home/katharina/SciFi/myassetstore.sst'
        self.__class__.storage.import_asset_store(path_to_asset_store)
        print('\n\n import asset !!!!!!\n\n',self.__class__.storage.get_all_assets(ids_only=1))
        self.__class__.storage.get("pdb3bik", assetToFile=1, fileextension=".txt")
        print(self.__class__.storage.asset_count())

    def test_open_metastore(self):
        path_to_asset_store = '/home/katharina/SciFi/myassetstore.sst'
        self.__class__.storage.import_asset_store(path_to_asset_store)
        path_to_metastore = 'PDB_Meta'
        self.__class__.storage.open_meta_store(path_to_metastore, "new_meta_results")
        self.__class__.storage.get_by_constraint('all', assetToFile=1, fileextension='.txt',metaToFile=1, fileextension_meta='.txt')
        print('\n\n openmeta  !!!!!!\n\n', self.__class__.storage.get_all_assets(ids_only=1))

    def test_open_metastore_at_start(self):
        storage2 = boost_scifi.Storage("asset_test2", "metatest_PDB")
        storage2.get_by_constraint('PDBID=400D', metaToFile=1, fileextension_meta='.txt')
        with open('/home/katharina/SciFi/PythonWrapper_Boost/results/metadata.txt') as file:
            txt = file.read()
            print(txt)


    #adding meta table means adding a table on top of existing, loading means deleting old one and using the new one?

    def test_add_meta_table(self):
        self.__class__.storage.add_meta_table("test_table", "key","key VARCHAR, my_name VARCHAR, my_value FLOAT")
        self.__class__.storage.get_by_constraint("priority>20", assetToFile=1,fileextension='.svg',metaToFile=1, fileextension_meta='.txt')
        with open('/home/katharina/SciFi/PythonWrapper_Boost/results/metadata.txt') as file:
            txt=file.read()
            self.assertIn('my_name', txt)
            self.assertIn('key', txt)

    def test_add_meta_table_advanced(self):
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/PDB_Parquet/metadata.csv'))
        storage1 = boost_scifi.Storage('asset_test', 'meta_test')
        storage1.add_meta_table_advanced('audit_author', 'PDBID',
                                         '/home/katharina/SciFi/PythonWrapper_Boost/PDB_Parquet/audit_author.csv', 0,
                                         'PDBID VARCHAR, name VARCHAR, pdbx_ordinal INT, identifier_ORCID VARCHAR')
        storage1.load_metadata_advanced_from_file('/home/katharina/SciFi/PythonWrapper_Boost/PDB_Parquet/metadata.csv', 1, 'PDBID VARCHAR', 0)
        storage1.add_meta_table_advanced('audit_author','PDBID','/home/katharina/SciFi/PythonWrapper_Boost/PDB_Parquet/audit_author.csv',0,'PDBID VARCHAR, name VARCHAR, pdbx_ordinal INT, identifier_ORCID VARCHAR')
        storage1.get_by_constraint(constraint='pdbx_ordinal>0', assetToFile=0, fileextension='.svg', metaToFile=1, fileextension_meta='.txt')

    def test_define_user_meta_cols(self):
        self.__class__.storage.define_user_meta_cols("my_name VARCHAR, my_value FLOAT",0)
        self.__class__.storage.get_all_assets(metaToFile=1,assetToFile=1)

    def test_insert_new_meta_row(self):
        self.__class__.storage.insert_new_meta_row('new_row,100,testing the insertion of a new row in metadata')
        self.__class__.storage.get_by_constraint('key=new_row', assetToFile=1, fileextension='.svg', metaToFile=1, fileextension_meta='.txt')
        with open('/home/katharina/SciFi/PythonWrapper_Boost/results/metadata.txt') as file:
            print(file.read())

    def test_load_metadata_advanced(self):
        path_csv = ('/home/katharina/SciFi/example_datasets/hamburgimages/usermeta_advanced.csv')
        self.__class__.storage.load_metadata_advanced_from_file(path_csv,skiplines=4, columns='ID VARCHAR, age INT, description VARCHAR')
        self.__class__.storage.get_by_constraint('age>0',assetToFile=1, fileextension='.svg', metaToFile=1, fileextension_meta='.txt')
        with open('/home/katharina/SciFi/PythonWrapper_Boost/results/metadata.txt') as metadata:
            metadata = metadata.read()
            self.assertIn('Karl', metadata)


    # def test_load_metadata_from_remote(self):
    # def test_load_metadata_from_remote_advanced(self):
    #def test_load_asset_from_remote(self):

if __name__ == '__main__':
    unittest.main()