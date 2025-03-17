# get first arg
term=$1

assert () {
    E_PARAM_ERR=98
    if [ -z "$1" ]
    then
        echo "No argument passed to assert function. Exiting..."
        exit $E_PARAM_ERR
    fi
}

assert $term

echo "Training term $term"

if [ $term -eq 0 ]; then
    python tools/run_net.py --config-file ./projects/ngp/configs/ngp_chair.py > logs/term0.log
    python tools/run_net.py --config-file ./projects/ngp/configs/ngp_drums.py > logs/term0.log

    python test0.py > logs/term0.log
else
    python tools/run_net.py --config-file ./projects/ngp/configs/ngp_ficus.py > logs/term1.log
    python tools/run_net.py --config-file ./projects/ngp/configs/ngp_hotdog.py > logs/term1.log
    python tools/run_net.py --config-file ./projects/ngp/configs/ngp_lego.py > logs/term1.log
    python tools/run_net.py --config-file ./projects/ngp/configs/ngp_mic.py > logs/term1.log

    python test1.py > logs/term1.log
fi

