import os

path = 'your/path'

tags = [
    'y0p1',
    'y0p2',
    'y0p3',
    'y0p4',
    'y0p5',
    'y0p6',
    'pt0p8',
    'pt1p0',
    'pt1p2',
    'pt1p4',
    'pt1p6',
    'pt1p8',
]

for item in tags:
    os.symlink(f'{path}/cum.cbwc.default.{item}.vz0X.root', f'./inputs/stat.{item}.root')
