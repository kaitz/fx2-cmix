# fx2-cmix
The fx2-cmix is a updated implementation of [fx-cmix](https://github.com/kaitz/fx-cmix). 


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

## General:
* Article order has changed. All REDIRECT articles removed from order. All articles about census data were moved to the end. All articles about images were moved after that. Redirects fall automatically after that. Some small tweaks.

## Transform:
* Wikipedia transfor is now a single pass version. There are some differences. Some html entities are converted to UTF8. Transform disk usage is reduced from about 18GB to 7GB and time from 7 to 3 min on a test computer.

Some C++ code is replaced with a simpler one (mostly in cmix), to keep executable size as small as possible.

## Additional documents
Link 1 contains possible changes timeline before submission. Link 2 contains improvment graphs. Link 3 contains a discussion after fx-cmix submission. The idea is to show the action plan that led to the final result.
1. https://docs.google.com/document/d/14nNIMAFC11lNFD-WrLQbGLIuRqqcKKBnUtrxy-tc0Rc/
2. https://docs.google.com/document/d/1DW0Lqr_y-yAIvPpGua56j1Y3gRaMS5yt2RuQy5xgGMk/
3. https://encode.su/threads/4161-fx-cmix-(HP)?p=82115&viewfull=1#post82115


# Results
Below is the fx2-cmix result:

| Metric | Value |
| --- | ----------- |
| fx2-cmix compressor's executable file size (S1)| 437082 bytes |
| fx2-cmix self-extracting archive size (S2)| 110954401 bytes |
| Total size (S) | 111391483 bytes |
| Previous record (L) | 112578322 bytes |
| fx2-cmix improvement (1 - S/L) | 1.054% |

| Experiment platform |  |
| --- | ----------- |
| Operating system | Ubuntu 20.04 LTS |
| Processor | Intel(R) Xeon(R) CPU @ 3.10GHz [Geekbench score 982](https://browser.geekbench.com/v5/cpu/22495686)|
| Memory | 16 GB |
| Decompression running time | 67 hours |
| Decompression RAM max usage | 9726156 KiB |
| Decompression disk usage | ~21GB |

Time, disk, and RAM usage are approximately symmetric for compression and decompression.
# Time
Must run below 70000/T h. T=982.
70000/982=66,8316666666667*50/71,2830957230143=46,8776404761905 h
46,9 h < 50 h

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


# Running fx2-cmix decompressor
The compressor is expected to output an executable file named `archive9` in the same directory (`./run`). The file `archive9` when executed is expected to reproduce the original enwik9 as a file named `enwik9_restored`. The executable file `archive9` should be launched without argments from the directory containing it.
```bash
cd ./run
./archive9
```

# Expected output on compression
```num models 469
197915 bytes -> 1123959 bytes in 240.13 s.
num models 469
100094 bytes -> 411996 bytes in 101.37 s.
preprocessing...
Detected block types: TEXT: 100.0%
Loaded 44515 words
num models 469
```
934014283 bytes -> 110xxxxxxx bytes in 240250.90 s.

# Expected output on decompression

