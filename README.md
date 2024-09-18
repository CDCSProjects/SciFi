# Welcome to SciFi
## An embedded asset store designed for scientific data

SciFi combines a relational database and a key-value store to store and query data (DataStore) and metadata (MetaStore) in an efficient way without requiring special user permissions or administration of the datatabase systems. The data is stored in a key-value store, which uses a folder on disc or in main memory. Within this folder, one or more \*.sst files are saved. This depends on the parameters you pass during the creation of the store. The metadata is stored in a \*.db or \*.parquet file. The identification between data and metadata is done via a unique key. The uniqueness of the key is enforced by design in the DataStore, and by defining it as UNIQUE PRIMARY attribute in the MetaStore. **Keys are case sensitive.**

The default backends are DuckDB for the relational database and RocksDB for the key-value store. However, you are free to integrate your own backends by extending the classes AssetStore (DataStore) and /or MetaStorage (MetaStorage). 

Table of Contents
=================

* [Definitions](#definitions)
* [State of Development](#state-of-development)
    * [SciFi on Conferences](#scifi-on-conferences)
* [Folder Structure](#folder-structure)
* [Build and Run](#build-and-run)
    * [Known issues](#known-issues)
    * [Build a Binary](#build-a-binary)
    * [Build the Python Wrapper](#build-the-python-wrapper)
    * [Use with PyTorch](#use-with-pytorch)
* [Examples](#examples)
* [Documentation](#documentation)
   
## Definitions

To make sure that the documentation is not confusing, we will provide our definition of a few central terms here.

**Asset** An asset is a piece of data which was originally stored in a file. This can be a picture, a text document, a compressed file, or any other kind of data.
 
**Metadata** Metadata is all data which is used to further describe the assets. Some metadata is always automatically created while ingesting assets from the file system. This includes the key, the file ending, if data is compressed (we currently support gz files), and the original folder where the data was stored. The user can add additional metadata. This can be done in two different ways: 1) The most simple way is to provide a tabular file (csv, tsv, or anything in this format). This can be done directly via a framework interface or via the shell. 2) The framework also provides an interface for executing SQL queries in case anything more specific is required for your use-case. User defined metadata can be used to filter and return the according assets.

**DataStore** The DataStore stores all assets in its own structure. For our standard backend, this structure are sorted string tables (sst). The abstract class *AssetStore* defines the interfaces which have to be implemented by the backend, and it implements all functionality which does not depend on a specific backend. The class *RocksStore* implements the standard backend using RocksDB.

**MetaStore** The MetaStore stores all automatically and user generated metadata. The abstract class *MetaStorage* defines the interfaces which have to be implemented by the backend, and it implements all functionality which does not depend on a specific backend. The class *DuckStore* implements the standard backend using DuckDB.

## State of Development

SciFi is currently in an early version which provides basic functionality, e.g. creating a storage from an existing folder and filtering assets by their metadata. You are, of course, free and encouraged to use this as a framework for your own AssetStore which is tailored to your needs without the need to redevelop the basic structures. However, there might still be bugs and the build process might not be as smooth as expected.
We will provide a more realistic example dataset as soon as we have figured out the legal aspect.

Currently, we provide some simple examples for the usage of SciFi as a framework or as a shell. All examples can be found in the folder *examples*.

~~There is a cython-based python wrapper which allows a direct access of the DataStore and the MetaStore. We are working on a wrapper which works with generic backends and which does not require a direct access. Our integration into PyTorch uses this wrapper to extend the Dataset class of Pytorch. This way, you can access all assets stored via SciFi while using PyTorch, like you would access any other map-style dataset. **If you are experienced in PyTorch and/or Cython, and are looking for a bachelor or master thesis topic at UHH or TUHH, drop us a message!**~~
The wrapper based on boost is in the testing branch and not necessarily up to date.

### SciFi on Conferences
- **19.09.22** Presentation of SciFi at "Machine Learning in Natural Sciences: from Quantum Physics to Nanoscience and Structural Biology" in Hamburg. You can find the abstract at https://www.conferences.uni-hamburg.de/event/256/contributions/728/
- **23.09.24** Presentation of SciFi at LWDA in Würzburg, Germany

## Folder Structure

- *backend* Contains headers and sources of the *DuckStore* and *RocksStore* classes. It is recommended that you also put your own backend sources here if you have some.
- *examples* Some examples that show how to use the framework and the shell. See below for further explanations.
- *framework* Contains the interfaces for the DataStore and the MetaStore and the implementation where possible.
- *helper_scripts* Scripts we use for experiments/benchmarks for our publications.
- *pythonwrapper* Contains the cython source for the python wrapper, a setup.py to build the wrapper, a Dataset extension for PyTorch, and example scripts.
- *shell* Contains the headers and sources for the extensible shell.

## Build and Run

We tested SciFi using Ubuntu in different versions and on different laptops and a Jetson Nano. Please make sure that the packages *libcurlpp-dev* and *build-essential*, or their counterpart in whatever distribution you are using, are installed. We used g++ as our c++ compiler. If you intend to use another compiler, please change the call accordingly in the script *build.sh*.

### Known Issues

**Linker or compiler errors to curl calls:** You are most likely missing the c++-development files for curl. While build-essential should be available on every machine which provides a compiler, the libcurlpp developer files are not always available, but can be downloaded and built in user space. Please visit http://www.curlpp.org/ for more information. 
 
**Compiler is not happy with the namespce *filesystem* or the template reduction or anything else that should work according to the C++17-standard:** Your compiler or libraries might not be up to date with the standard. Try to update your compiler and *build-essential*. In case you use an older distribution, you might have to use an archived repository or build a new compiler yourself. We used g++ v. 10.3. Make sure that the new compiler is actually called, i.e. that it is set as the standard c++-compiler. Note: Even if your current version pretends to support c++17, this support might only be partial or experimental. We've experienced this with older g++ versions.

**During library build: Linker errors caused by rocksdb which say \"recompile with -fPIC\":** Although the RocksDB library should have been built as a portable version using -fPIC, this did not always happen. The most simple way to solve this issue is to add the line *CXXFLAGS += -fPIC* to the Makefile of rocksdb and rebuild the library by calling *PORTABLE make static_lib* from the rocksdb folder.

### Build a Binary

*downloadAndBuild.sh* is a script for downloading and building the static libraries for rocksdb and duckdb, and for building a simple example shell which can be found in examples/SimpleStore.cpp. It has been tested with duckdb v0.4.1-dev1214 and rocksdb v. 7.5.0 on several notebooks and a Jetson Nano. After a successful build, you will find a binary called *simple* in the root folder of SciFi. Run this binary to be greeted with a nice little shell.

### Build the Python Wrapper (old cython version)

**Note: The python wrapper does not support the central *Storage*-Interface. For this reason we do not advise to use it directly until a new version is released. This wrapper is mainly used for the extension if the Dataset class in PyTorch. We highly recommend to use that instead.**
1. Change to the directory *pythonwrapper*
2. Execute the following command to build a library in the current directory: *python setup.py build_ext --inplace*. In case of a rocksdb-related linker error, refer to the section *Known Issues*.
In case python3 is not set as your default python call, use *python3 setup.py build_ext --inplace*.
3. Your library will have the name *scifi.cpython-38-\<architecture\>-linux-gnu.so*. You can now either install this library or use it directly from the same folder. The module is called *scifi*. It provides the classes *rocks* and *duck*. 

### Use with PyTorch (old version)

You can access your SciFi data directly without writing the assets back to the file system or using the original files.
 
Prerequisites: Make sure that the SciFi module is available, i.e. you either installed the python wrapper, or the library file is in the same folder as your script. See above for instructions on how to build the wrapper.

Change to the folder *pythonwrapper*. The file SciFiDataset.py contains the extension of the *Dataset* class. The file DatasetExample.py shows examples of how you can use it. At least two steps are necessary:
1. Create a dataset:
*scifi_dataset = SciFiDataset(datasetname="DataName", metaname="MetaName")*.
*DataName* refers to the name of your DataStore, i.e. the name of the folder where your sst file(s) are stored.
*MetaName* refers to the name of your MetaStore, i.e. the name of your db or parquet file.
2. You can now access your data in map-style:
*sample=scifi_dataset[\"myKey\"]*.
*myKey* refers to a unique key.

The definition of what the *len* function is supposed to do in the *Dataset* class is fuzzy. We decided to use it to return the number of assets in the DataStore.

Since the keys in SciFi can contain arbitrary characters and are not limited to integers, the dataset can not be iterated naively as shown in some tutorial about map-style datasets. To solve this, we introduced the function *getIDsByIndex(lower,upper)*. This will return a list of all indexes within the range [lower, upper] sorted alphabetically. The resulting list can be used for iteration. We are working on sorting by arbitrary attributes and an iteration-style dataset.

If you want to iterate over all assets with certain metadata, you may use the function *getIDsByMeta(metadataconstraint)*, where *metadataconstraint* is any constraint or combination of constraints, e.g. \"number>10\" or \"number>10 AND text like '%blubb%'\".

Note that the direct access via a key only uses the DataStore, while the *len* and *getIDsByIndex* functions require the according MetaStore. This is because the DataStore can only provide an estimate of the number of its assets. An accurate result would require a more performance intensive count. 

## Examples

All examples can be found in the *examples* folder.

- *SimpleStore.cpp* shows a minimal example to create a store and a shell, and to tun the shell
- *ASDemo.cpp* shows how to add functions to the shell and how to access the store directly + some time measurement. **NOTE** The source of the input files here is hard coded, change the path to something that exists on your drive.
- All other examples were used for our paper to demonstrate the speed of different operations, e.g. writing datasets to main memory.

## Documentation
Currently, the documentation consists of this readme, *some* code documentation in Doxygen style, and a user documentation which can be found in the file SciFiFirstSteps.pdf in the root folder of this repository. We are aware that the documentation needs an update.

