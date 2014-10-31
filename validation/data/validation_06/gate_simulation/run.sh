
NB_RUN=50

rm -rf run_*
rm -f benchmarkVRTCT_*.dat
rm -f benchmarkVRTCT.root
rm -f timing.dat

for (( i = 0; i < $NB_RUN; i++ ))
do
    # Create first seed
    SEED1=`echo $[ 1 + $[ RANDOM % 100000000 ]]`

    # Create second seed
    SEED2=`echo $[ 1 + $[ RANDOM % 100000000 ]]`

    # Create output.mac file
    cat output.start > output.mac
    echo "/gate/random/setEngineSeed $SEED1"        >> output.mac
    echo "/gate/output/imageCT/setStartSeed $SEED2" >> output.mac

    echo Start Run $i
    echo "********************************************************************************" >> timing.dat
    echo Start >> timing.dat
    echo Run $i >> timing.dat
    date >> timing.dat

    /usr/local/share/gate_v6.2/bin/Gate < benchmarkVRTCT.mac 2>/dev/null >/dev/null
    date >> timing.dat
    echo Stop >> timing.dat
    echo "********************************************************************************" >> timing.dat
    echo >> timing.dat
    echo >> timing.dat
    echo >> timing.dat
    echo >> timing.dat
    echo >> timing.dat

    mkdir run_$i
    cp benchmarkVRTCT_*.dat run_$i
    cp benchmarkVRTCT*.root run_$i
    mv output.mac run_$i
done

xclock

