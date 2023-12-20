import unittest
import boost_scifi
import os

class SciFiWrapTest(unittest.TestCase):

    storage = boost_scifi.Storage('rocksdb', 'duckdb')
    def setUp(self):
            self.__class__.storage.load_metadata_from_file('/home/katharina/SciFi/example_datasets/hamburgimages/usermeta.csv', 'key')
            self.storage.load_assets_from_dir('/home/katharina/SciFi/example_datasets/hamburgimages/assets')
          #  self.ids = ['flag', 'ship']
           # self.all_ids = ['flag', 'lighthouse', 'ship', 'wikipedia', 'wittenberger_heide']
          #  self.result_path = '/home/katharina/SciFi/PythonWrapper_Boost/results/'

    def test_load_asset_from_dir(self):
        #TODO momentan keine Fehlermeldung wenn bspw. Pfad nicht korrekt, überprüfen ob input funktioniert

        self.__class__.storage.load_assets_from_dir('/home/katharina/SciFi/example_datasets/hamburgimages/assets')

    def test_load_metadata_from_file(self):
        #TODO same as above, check if successful difficult because no assertion is thrown by incorrect path
        path_csv = '/home/katharina/SciFi/example_datasets/hamburgimages/usermeta.csv'
        self.__class__.storage.load_metadata_from_file('/home/katharina/SciFi/example_datasets/hamburgimages/usermeta.csv', 'key')

    def test_get_single_id(self):
        #test return string
        result = self.__class__.storage.get('wikipedia')
        file = open('/home/katharina/SciFi/example_datasets/hamburgimages/assets/wikipedia.txt')
        self.assertEqual(result, file.read())
        file.close()
        #TODO abprüfen von Rückgabe, momentan unterscheiden sich strings nur in newline symbol..

    def test_get_single_id_to_file(self):
        #test return to file for single id
        self.__class__.storage.get('wikipedia', assetToFile=1, fileextension='.svg', metaToFile=1, fileextension_meta='.txt')
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/wikipedia.svg'))
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/wikipediameta.txt'))
        os.remove('/home/katharina/SciFi/PythonWrapper_Boost/results/wikipedia.svg')
        os.remove('/home/katharina/SciFi/PythonWrapper_Boost/results/wikipediameta.txt')

    def test_get_multiple_ids(self):
        #test return string for single id
        #TODO same as above, rückgabewert überprüfen
        result = self.__class__.storage.get(['flag', 'wikipedia'])




    def test_get_multiple_ids_to_file(self):
        #test return to file for multiple ids
        ids = ['flag', 'wikipedia']
        result_path = '/home/katharina/SciFi/PythonWrapper_Boost/results/'
        fileextension = '.svg'
        self.__class__.storage.get(ids, assetToFile=1, fileextension='.svg', metaToFile=1, fileextension_meta='.txt')
        self.assertTrue(os.path.isfile(result_path+ids[0]+fileextension))
        self.assertTrue(os.path.isfile(result_path+ids[1]+fileextension))
        self.assertTrue(os.path.isfile(result_path+'metadata.txt'))
        os.remove(result_path+ids[0]+fileextension)
        os.remove(result_path+ids[1]+fileextension)
        os.remove(result_path+'metadata.txt')

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
            os.remove(file_path)

        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/metadata.txt'))
        os.remove('/home/katharina/SciFi/PythonWrapper_Boost/results/metadata.txt')


    def test_remove_asset(self):
        id = 'ship'
        self.__class__.storage.remove_asset(id, remove_metadata=1)
        current_ids = self.__class__.storage.get_all_assets(ids_only=1)
        self.assertNotIn(id, current_ids)

    def test_filter(self):
        filter = "priority>20"

        self.assertEqual(['ship','flag'], self.__class__.storage.get_IDs_by_constraint(filter))

        self.__class__.storage.get_by_constraint(filter,assetToFile=1, fileextension='.svg')
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/ship.svg'))
        self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/flag.svg'))
        os.remove('/home/katharina/SciFi/PythonWrapper_Boost/results/flag.svg')
        os.remove('/home/katharina/SciFi/PythonWrapper_Boost/results/ship.svg')

        self.__class__.storage.store_filter(filter, "prio", "every asset with priority higher 20")
        self.assertIn('prio', self.__class__.storage.get_all_filters())
        ids = self.__class__.storage.apply_filter('prio', assetToFile=1, fileextension='.svg')

        for i in range(len(ids)):
            self.assertIn(ids[i],['ship','flag'])

        #self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/ship.svg'))
        #self.assertTrue(os.path.isfile('/home/katharina/SciFi/PythonWrapper_Boost/results/flag.svg'))
        #os.remove('/home/katharina/SciFi/PythonWrapper_Boost/results/flag.svg')
        #os.remove('/home/katharina/SciFi/PythonWrapper_Boost/results/ship.svg')
        self.assertEqual('every asset with priority higher 20', self.__class__.storage.get_filter_text('prio'))

        self.__class__.storage.remove_filter('prio')
        self.assertNotIn('prio', self.__class__.storage.get_all_filters())





if __name__ == '__main__':
    unittest.main()