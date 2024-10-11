# fx2-cmix
The fx2-cmix is a updated implementation of [fx-cmix](https://github.com/kaitz/fx-cmix). 

[Archive](https://drive.google.com/file/d/14QillUEElT5vR0ttmayRAXlciXuPwDWm/)

Prize awarded on October 8, 2024. http://prize.hutter1.net/

# Submission Description
This submission contains fallowing major modifications on top of the recent fx-cmix Hutter Prize winner:
* NLP (Natural language processing)
* online reverse dictionary transform
* single pass wikipedia transform
* updated order of articles.

## More detailed changes

### cmix changes:
* mixers contexts are more similar to fxcm mixer contexts.
* mixers have weight update skipping when error is below threshold (improves speed). 
* removed the weight regularizer from the mixer (improves speed).
* executable binary size reduced due to "simpler" code.
* Removed 7 indirect nonstationary predictors, 6 match model predictors, 3 mixers. This improves compression time and at the same time allows fxcm to be more complex and slower.

### fxcm changes:
* Reverse dictionary transform. We load the dictionary when it is found after decompressing it. Text has a separate buffer from coded byte stream buffer.
* Natural language processing using stemmer (from paq8px(d)).
* Stemmer has new word types: Article, Conjunction, Adposition, ConjunctiveAdverb.
* Some word (related) contexts are changed based on what type of word was last. Some words are removed from word streams depending on the last word type. This improves compression.

There are four word streams:
* 1. basic stream of undecoded words.
* 2. decoded word stream after stemming for sentences. Contains all words. Reset when sentence ends.
* 3. decoded word stream after stemming for paragraphs. Contains words that are not: Conjunction, Article, Male, Female, ConjunctiveAdverb. Reset when paragraph ends.
* 4. decoded word stream after stemming. Contains words that are not: Conjunction, Article, Male, Female, Adposition, AdverbOfManner, ConjunctiveAdverb.
* Word limit per stream is increased from 64 to 256 words.
* New context that uses stemmer and decoded plaintext. Some global context are changed when word type is: \
ConjunctiveAdverb or Conjunction - skip updating in stream 1. Conjunction for sentence reset. etc. Knowing these new words allowed large amounts of compression improvements. 
* In some cases words are removed between certain chars from stream 2 and 3 when following is true:  \
=| - wiki template \
<> - html/xml tags \
[| - wiki links \
() - usually words in sentences
* Main predictors are split between three different ContextMaps. This provides better compression. Sizes for hash tables are 32, 64 (standard for paq8 versions) or 128 bytes per contexts. 32 byte size is good for small memory context (below 256 KB), 64 is good for medium sized context (up-to 16MB), 128 is good for large memory context (more than 16MB).
* One state table is removed and replaced with another one. State tables are generated at runtime to reduce code size.
* Added sparse match model. With a gap of 1-2 bytes and minimum length of 3-6 bytes. Mostly for escaped UTF8.
* Detection of math, pre, nowiki, text tags in decoded text stream. Some word related contexts are not used when 3 first tags content is compressed. Improves compression speed.
* More parsing of lists and paragraphs. So that context for predictors is best as they can.
* Optimized context skipping in main predictors.
* Main predictor context bias is not forwarded to cmix floating point mixers, instead a single prediction bias is set. This avoids unnecessary expansion of mixer weight space and maintains lower memory/cpu usage. There are some other predictions that are not forwarded as they make compression worse and slower.
* Some mixers and APM’s context size is larger so that prediction can be better.
* cmix LSTM expected byte is used for one mixer context, also LSTM prediction itself (as in paq8px(d)).
* partially/fully decoded word index into dictionary is used as a context for mixer in fxcm mixer/cmix floating point mixer.
* Some variables are renamed for better readability.

## Article order:
Here is a description of the pipeline used to generate the new article order:
1. A 1024-dimension embedding is created for each Wikipedia article using the [voyage-large-2-instruct](https://blog.voyageai.com/2024/05/05/voyage-large-2-instruct-instruction-tuned-and-rank-1-on-mteb/) model. Some article processing is done to extract more meaningful text. Jupyter notebook: src/article_order/embeddings.ipynb
1. Using t-SNE, the embeddings are reduced to a single dimension. Jupyter notebook: src/article_order/tsne.ipynb
1. Sort the articles based on the single dimension: sort -t$'\t' -n -k 2 before-sort.tsv > after-sort.tsv
1. Run k-means clustering based on the single dimension. Within each cluster, sort articles based on their original order (numerically increasing). Jupyter notebook: src/article_order/kmeans.ipynb
1. Reverse the order: tac final-clustered.txt > final-clustered-rev.txt
1. Do some manual sorting. Certain article types (e.g. images, disambiguation pages, etc) are moved to the end of the order. Jupyter notebook: src/article_order/manual-sort.ipynb

## Transform:
* Wikipedia transform is now a single pass version. There are some differences. Some html entities are converted to UTF8. Transform disk usage is reduced from about 18GB to 7GB and time from 7 to 3 min on a test computer.

Some C++ code is replaced with a simpler one (mostly in cmix), to keep executable size as small as possible.

## Additional documents
Link 1 contains possible changes timeline before submission. Link 2 contains improvment graphs. Link 3 contains a discussion after fx-cmix submission. The idea is to show the action plan that led to the final result.
1. https://docs.google.com/document/d/14nNIMAFC11lNFD-WrLQbGLIuRqqcKKBnUtrxy-tc0Rc/
2. https://docs.google.com/document/d/1DW0Lqr_y-yAIvPpGua56j1Y3gRaMS5yt2RuQy5xgGMk/
3. https://encode.su/threads/4161-fx-cmix-(HP)?p=82115&viewfull=1#post82115

# Authors
* Kaido Orav
* Byron Knoll

# Google Cloud Compute Engine parameters
Machine configuration
```
Machine type: c2-standard-4
CPU platform: Intel Cascade Lake
Minimum CPU platform: None
Architecture: x86/64
vCPUs to core ratio:  1 vCPU per core
Custom visible cores: —
Display device: Disabled
GPUs: None

Boot disk
Image: ubuntu-2004-focal-v20240731
Description: Canonical, Ubuntu, 20.04 LTS, amd64 focal image built on 2024-07-31

RAM 16GB
HDD 50GB
Intel(R) Xeon(R) CPU @ 3.10GHz
```
# Results
Below is the fx2-cmix result:

| Metric | Value |
| --- | ----------- |
| fx2-cmix compressor's executable file size (S1)| 441463 bytes |
| fx2-cmix self-extracting archive size (S2)| 110351665 bytes |
| Total size (S) | 110793128 bytes |
| Previous record (L) | 112578322 bytes |
| fx2-cmix improvement (1 - S/L) | 1,585% |

| Experiment platform |  |
| --- | ----------- |
| Operating system | Ubuntu 20.04 LTS |
| Processor | Intel(R) Xeon(R) CPU @ 3.10GHz [Geekbench score 1026](https://browser.geekbench.com/v5/cpu/22745877)|
| Memory | 16 GB |
| Decompression running time | 65 hours |
| Decompression RAM max usage | 9523660 KiB |
| Decompression disk usage | ~21GB |

Time, disk, and RAM usage are approximately symmetric for compression and decompression.
# Time
Must run below 70000/T h. T=1026.
70000/1026=68,2261208576998 (max time)
64,8166666666667*50/68,2261208576998=47,5013571428571 h
47,5 h < 50 h

# Instructions
The installation and usage instructions for fx2-cmix are the same as for fast-cmix.

One important note: it is recommended to change one variable in the source code for PPM. From line 26 in src/models/ppmd.cpp:

```
// If mmap_to_disk is set to false (recommended setting), PPM will only use RAM
// for memory.
// If mmap_to_disk is set to true, PPM memory will be saved to disk using mmap.
// This will reduce RAM usage, but will be slower as well. *Warning*: this will
// write a *lot* of data to disk, so can reduce the lifespan of SSDs. Not
// recommended for normal usage.
bool mmap_to_disk = true;
```

This variable is set to true by default, to comply with the Hutter Prize RAM limit.

# Installing packages required for compiling fx2-cmix compressor from sources on Ubuntu
Building fx2-cmix compressor from sources requires clang-17, upx-ucl, and make packages.
On Ubuntu, these packages can be installed by running the following scripts:
```bash
./install_tools/install_upx.sh
./install_tools/install_clang-17.sh
```

# Compiling fx2-cmix compressor from sources
A bash script is provided for compiling fx2-cmix compressor from sources on Ubuntu. This script places the fx2-cmix executable file named as `cmix` in `./run` directory. The script can be run as
```bash
./build_and_construct_comp.sh
```

# Running fx2-cmix compressor
To run the cmix-hp compressor use
```bash
cd ./run
cmix -e <PATH_TO_ENWIK9> enwik9.comp
```
`enwik9.comp` is used to store intermediate data output. The final decompressor will be created as a file named `archive9`.

# Running fx2-cmix decompressor
The compressor is expected to output an executable file named `archive9` in the same directory (`./run`). The file `archive9` when executed is expected to reproduce the original enwik9 as a file named `enwik9_restored`. The executable file `archive9` should be launched without argments from the directory containing it.
```bash
cd ./run
./archive9
```

# Expected output on compression
```num models 461
201043 bytes -> 1094862 bytes in 226.99 s.
num models 461
100088 bytes -> 411996 bytes in 94.24 s.
preprocessing...
Loaded 44515 words
num models 461
934220400 bytes -> 110111245 bytes in 228589.79 s.
```

# Expected output on decompression
```Loaded 44515 words
num models 461
Cmix decompression finished

110111245 bytes -> 934220400 bytes in 229670.44 s.
```
