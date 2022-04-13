import scipy.io.wavfile
import wave
import subprocess
import os

indir = "./convert"
outfile = 'outputSound.h'

BOARD_SAMPLERATE = 8000 # board is 8khz sampling

file_object = open(outfile, 'w')
for filename in os.listdir(indir):
    print(filename)
    dst = "wav.wav"

    process = subprocess.run(['ffmpeg', '-i', os.path.join(indir,filename), dst])

    with wave.open(dst) as fd:
        params = fd.getparams()
        frames = fd.readframes(100000000000000) # there better not be more than this many frames

    print(params)

    rate, data_np_ary = scipy.io.wavfile.read(dst)
    # print(data_np_ary.tolist())

    elementsTotal = 0
    count = 0
    for elem in data_np_ary:
        count+=1;
        if count%(params.framerate//BOARD_SAMPLERATE) == 0:
            elementsTotal+=1
    file_object.write('const int '+filename[0:-4]+'['+str(elementsTotal)+']')
    file_object.write(' = {')

    count = 0
    elements = 0
    for elem in data_np_ary:
        count+=1;
        if count%(params.framerate//BOARD_SAMPLERATE) == 0:
            elements+=1
            if (params.nchannels == 2):
                if(elem[0] != 0 or elem[1] != 0):
                    file_object.write(elem[0].astype(str) +',')
                    if count%((params.framerate//BOARD_SAMPLERATE)*20) == 0: # 20 LR samples per line
                        file_object.write('\n')
            else:
                file_object.write( elem.astype(str) + ',')
                if count%((params.framerate//BOARD_SAMPLERATE)*20) == 0: # 20 LR samples per line
                    file_object.write('\n')

    file_object.write('0};\n')

    os.remove("wav.wav")
    print("created array of " +str(elementsTotal) +" elements")

file_object.close()
