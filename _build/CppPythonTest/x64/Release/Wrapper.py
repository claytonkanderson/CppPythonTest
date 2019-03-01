import numpy as np
import gym
import matplotlib.pyplot as plt

from time import time

from keras.models import Sequential, Model
from keras.layers import Dense, Activation, Flatten, Input, Concatenate
from keras.optimizers import Adam
from keras.callbacks import TensorBoard

from rl.random import OrnsteinUhlenbeckProcess, GaussianWhiteNoiseProcess

from InterfaceOutline import Memory, Agent, Simulation
from BrainDDPG import DDPGBrain

################################################################################

class Wrapper :
	def __init__(self) : 
		
		# Get the environment and extract the number of actions.
		np.random.seed(123)
		
		self.numActions = 2
		self.numStateParams = 6
		
		actorInputs = Input(shape = (self.numStateParams,))
		x = Dense(32, activation='relu')(actorInputs)
		x = Dense(32, activation='relu')(x)
		actions = Dense(self.numActions, activation='tanh')(x)
		actor = Model(inputs=actorInputs, outputs = actions)
		
		stateInput = Input(shape = (self.numStateParams,))
		actionInput = Input(shape = (self.numActions,))
		x = Concatenate()([stateInput, actionInput])
		x = Dense(32, activation='relu')(x)
		x = Dense(32, activation='relu')(x)
		x = Dense(32, activation='relu')(x)
		values = Dense(1, activation='linear')(x)
		critic = Model(inputs=[stateInput, actionInput], outputs = values)
		
		randomProcess = OrnsteinUhlenbeckProcess(size=self.numActions, theta=.15, mu=0., sigma=.3)
		brainMemory = Memory(50000)
		
		brain = DDPGBrain(actorModel=actor, criticModel=critic, memory = brainMemory, randomProcess = randomProcess, gamma = 0.99,
				targetModelUpdate=1e-3, batchSize = 32)
		brain.Compile(optimizer=Adam(lr=1e-3, clipnorm=1.))
		
		self.Agent = Agent(brain)

def Initialize() : 
	return Wrapper()

def GetAction(wrapper, state) :
	return wrapper.Agent.Brain.SelectAction(state)

def Update(wrapper, experience) :
	state, action, nextState, reward = experience
	sasr = np.array([state,action,nextState,reward]).flatten()
	wrapper.Agent.ProcessSARS(sars)
	error = wrapper.Agent.Brain.Train()

def Terminate(wrapper) :
	# TODO : Save / Visualize
	return 0
