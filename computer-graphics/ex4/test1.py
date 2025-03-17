from ast import parse
import os
import jittor as jt
# from train import Trainer
# from model import NerfNetworks, HuberLoss
from tqdm import tqdm
# from utils.dataset import  NerfDataset
import argparse
import numpy as np
import os
from jnerf.runner import Runner
from jnerf.utils.config import init_cfg, get_cfg
# jt.flags.gopt_disable=1
jt.flags.use_cuda = 1


def main(config, ck_file):

    init_cfg(config)
    cfg = get_cfg()
    cfg.ckpt_path = ck_file
    cfg.dataset_dir = 'data/nerf_synthetic/'+cfg.exp_name

    runner = Runner()
    runner.test(True)


if __name__ == "__main__":
    objects = ['ficus', 'hotdog', 'lego', 'mic']
    base_dir = 'logs/test/'
    for obj in objects:
        config = base_dir + f'{obj}/' + f'ngp_{obj.lower()}.py'
        ck_file = base_dir + f'../{obj}/' + 'params.pkl'
        main(config, ck_file)
