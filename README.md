# Make Acceptance Scan

author: yghuang

version: 2.0

date: 08.10.2024

## Quick Guid

1. Use `linkBatch.py` and make soft links of root files of acceptance scanning
    * If the path of root files is not the same as mine, one should do step 1 manually

    * For example, `inputs/stat.y0p5.root`

2. Compile the executable with `make`
    * If you are using a new version `CumulantCalculation` package (>7.0), activate `ALTER=1` to include all kinds of ratios (from both method1 and method2)

3. Run `./scan path tag cent mode scan order`
    * `path` can be `inputs` if you are following my step

    * `tag` can be `stat` or `sys` or any other tags you like, but make sure the file name formatting follows `path/tag.scanTag.root`

    * `cent` can be `0` to `8`, for most central to peripheral

    * `mode` can be `y`/`rap`/`rapidity` for rapidity scan and `pt`/`pT` for pT scan

    * `scan` can be `0` for accumulative scan and `1` for differential scan

    * `order` can be `4` or `6`, indicating the highest order of cumulants

4. After that, one can find something like `outputs/stat.cent0.y.root` the exact file name of which depends one your arguments

## Patch Note

08.10.2024 by yghuang (v2.0):

1. The arglist is changed: `./scan path tag cent mode scan order`, for details see the guide

2. The CBWC methods for ratios are changed, define `ALTER=1` to activate

03.10.2024 by yghuang (v1.1):

> Add differential rapidity scan (use mode code: 2)
> Add one more argument `order`

02.07.2024 by yghuang (v1.0):

> First version
