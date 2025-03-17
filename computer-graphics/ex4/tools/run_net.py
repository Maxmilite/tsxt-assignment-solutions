from jnerf.runner import Runner
from jnerf.utils.config import init_cfg
import numpy as np
import argparse
from tqdm import tqdm
import jittor as jt
from ast import parse
import os
os.environ['CUDA_VISIBLE_DEVICES'] = '0'
# from train import Trainer
# from model import NerfNetworks, HuberLoss
# from utils.dataset import  NerfDataset
# jt.flags.gopt_disable=1
jt.flags.use_cuda = 1

# set default dtype to float16


def main():
    assert jt.flags.cuda_archs[0] >= 61, "Failed: Sm arch version is too low! Sm arch version must not be lower than sm_61!"
    parser = argparse.ArgumentParser(
        description="Jittor Object Detection Training")
    parser.add_argument(
        "--config-file",
        default="",
        metavar="FILE",
        help="path to config file",
        type=str,
    )
    parser.add_argument(
        "--task",
        default="train",
        help="train,val,test",
        type=str,
    )
    parser.add_argument(
        "--save_dir",
        default="",
        type=str,
    )

    args = parser.parse_args()

    assert args.task in [
        "train", "test", "render"], f"{args.task} not support, please choose [train, test, render]"
    if args.config_file:
        init_cfg(args.config_file)

    runner = Runner()

    if args.task == "train":
        runner.train()
    elif args.task == "test":
        runner.test(True)
    elif args.task == "render":
        runner.render(True, args.save_dir)


if __name__ == "__main__":
    main()
