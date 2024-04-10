Applied data for BurstFilter.

For the details about how to use the data, see ```src/util/read_dataset.cpp```.

### Original Data

We collected and augmented these data ourselves; hence, they can be open-sourced without any concerns regarding copyright issues.

#### Phone Dataset

These data are used for jitter tests.

- ```wechat_uniform_1.dat```: Data file.
  - Format: Data Format: Binary reading, with each record comprising 21 bytes: 13 bytes for the quintuple and 8 bytes for the timestamp (double, unit: seconds).
  - Quintuple: SrcIP + DstIP + SrcPort + DstPort + Protocol (maintaining the original byte order, i.e., big-endian), where IP addresses and port numbers have been anonymized using random values.
  - Timestamp: Little-endian, double precision, unit: seconds.

- ```ground_truth_uniform_1.txt```: Ground truth of jitters for each stream.
  - Data Format: Jitter Flag (1 for Yes / 0 for No) + SrcIP + DstIP + SrcPort + DstPort (same as above, in big-endian) + Start Time of Flow + End Time of Flow (+ Start Time of Jitter + End Time of Jitter, if available) (little-endian double, unit: seconds)

### Cited Dataset

These data are not original to us; rather, they were obtained through application processes, hence, we are unable to open-source them. Here, we merely acknowledge the data providers and release the code we employed for data processing.

#### Internet Dataset

Obtained from [The CAIDA UCSD Anonymized Internet Traces Dataset - 2018.03.15.](http://www.caida.org/data/passive/passive_dataset.xml)

We picked the first 27,121,712 traces for the ExpBurst Detection and Cache Prefetching tests.

#### Cloud Dataset

Obtained from [Yahoo! Webscope dataset G4 - Yahoo! Network Flows Data, version 1.0.](https://webscope.sandbox.yahoo.com/catalog.php?datatype=g)

We picked the first package with 10,065,284 traces for the Abnormal Latencies tests.

#### Temperature Dataset

Obtained from [Detecting outliers in a univariate time series dataset using unsupervised combined statistical methods: A case study on surface water temperature. *Ecological Informatics*](https://github.com/AtmosferaUSM/mukahead_ML_water-temperature_outlier_detection/tree/main/data)

We multiplied and divided the data since the original data is small.