project 2 checkpoint
There is the check.c for checkpoint.
For "Input handling 10", gcc -o check check.c
Then run ./check.
The program will ask you to insert the name of imagefile(mnist/train-images-idx3-ubyte) and the index from 0 to 59999.

In the proj2.c, it will read and store every training data to do further processing.

For "Simple perceptrons", 
gcc -o proj2_single proj2_single.c -lm
Then run ./proj2_single <epoch> <learningrate> mnist/train-labels-idx1-ubyte mnist/train-images-idx3-ubyte
In this program, there are two processes.
The first process is implementing single perceptron "is a three".
The trainlabel and trainpixel store the data from train- file respectively.
For example, trainlabel[3] stores the label of trainpixel[3][0~783].
At first this program reads data from mnist.
And then it will forward the data through the single perceptron with updating weights.
For every 5000 training, it will show you the accuracy.

The second process is for the perceptron of "winner".
It will contain 10 output perceptrons.
If the target value is 3, then the output perceptrons should be (0,0,0,1,0,0,0,0,0,0).
It also does same thing as the first process does but with the 10 output perceptrons.
This process will show you the accuracy.


For "Multilayer neural network"
gcc -o proj2 proj2.c -lm
Then run ./pron2 <epoch> <learningrate> <layernum> mnist/train-labels-idx1-ubyte mnist/train-images-idx3-ubyte mnist/t10k-labels-idx1-ubyte mnist/t10k-images-idx3-ubyte
At first, it will read and store all the data from train-* and t10k-*.
After initializing some variables for multilayer neural network, it will forward the data from images.
The variable neuron and weight are used for forwarding where neuron[0] stores the input and neuron[i] (i>0) stores the y_i.
While forwarding, I didn't store the x_i but the y_i directly using sigmoid function.
Becuase x_i is not used for this project.
After forwarding, the backpropogation will be processed.
From the output to the hidden layer , from hidden layer to input layer  and so on, the weights will be updated.
These whole forwarding and backpropogation will be done by every train image, which means 60,000 times


After the training, it will test itself with testdata.
It will read and store the data from testfile not the trainingfile.
By forwarding, this program will classify the every test image with its label.
For test, it does not backpropogate.
This forwarding will be done by every t10k image, which means 10,000 times.
So 70,000 times of training and test will be done with 1 epoch.
Every 5000 training and 2000 test, this program will show you the accuracy about comparing classification with correct label.




At the very end of proj2, I tried to find "golden-rate number" to make the accuracy high.

Epoch  learningrate  layernum      	test		training
5	0.5		10		69.98%		71.02%
5	0.8		10		73.35%		74.15%
5	0.9		10		29.78%		33%

10	0.5		10		64.18		66.3
10	0.8		10		67.85		69.22
10	0.9		10		27.75		28.88


When epoch increases, it looks like the accuracy doesn't increase with these 6 experiment.

About learningrate, when it grows from 0.8 to 0.9 then the accuracy falls down.
That means the cost function value fluctuates because the learningrate is high.
If the cost function gets near the local maximum, with high learningrate (with big step), it can't easily go down / up to the maximum but to opposite of maximum.
If with small learningrate(with small step), it also can't easily get to the maximum.
So we need to find proper learningrate not too high but not too small.

Epoch  learningrate  layernum      	test		training
10	0.5		10		64.18		66.3
10	0.8		10		67.85		69.22
10	0.9		10		27.75		28.88


10	0.5		100		80.56		83.54
10	0.8		100		82.87		85.48
10	0.9		100		82.9		80.16

The accuracy increases when layernum increases.
But the computation time also increases.
Since this is fully connected multilayer neuron network, there are layernum*785 + (layernum+1)*10 weights.
So I set the maximum layernum as 100.



Epoch  learningrate  layernum      	test		training
10	0.5		100		80.56		83.54
10	0.8		100		82.87		85.48
10	0.9		100		82.9		80.16

20	0.5		100		81.83		83.17
20	0.8		100		84.56%		85.00
20	0.9		100		83.48		82.38

50	0.5		100		82.68		83.50
50	0.8		100		83.90		86.18
50	0.9		100		81.68		82.38

The accuracy of training increases when the epoch increases.
But the accuracy of test does not increase when the epoch increses.
That means the network is overeducated to training data not the test.
So with high epoch, we can get high accuracy in training not test.

So my golden-rate number is 20 0.8 100.



