# coding: utf-8

from transitions.extensions import GraphMachine


class Model(object):
    pass


states = [
    'Waiting inputs',
    'Waiting inputs in <num>',
    'Rejected',
    'Rejected while matching with subset',
    'Accepted',
    'Accepted in <num>',
    'Accepted in <any>'
]

transitions = [
    # Waiting inputs
    {
        'trigger': 'Empty Input',
        'source': states[0],
        'dest': states[2]
    },
    {
        'trigger': 'Match with last <num>',
        'source': states[0],
        'dest': states[5]
    },
    {
        'trigger': 'Match with <any>',
        'source': states[0],
        'dest': states[6]
    },
    {
        'trigger': 'Match with last keyset',
        'source': states[0],
        'dest': states[4]
    },
    {
        'trigger': 'Match with <num>',
        'source': states[0],
        'dest': states[1]
    },
    {
        'trigger': 'Match with keyset',
        'source': states[0],
        'dest': states[0]
    },
    {
        'trigger': 'Match with subset',
        'source': states[0],
        'dest': states[3]
    },
    {
        'trigger': 'Unmatched',
        'source': states[0],
        'dest': states[2]
    },

    # Waiting inputs in <num>
    {
        'trigger': 'Input numbers',
        'source': states[1],
        'dest': states[1]
    },
    {
        'trigger': 'Input others (ε-transitions)',
        'source': states[1],
        'dest': states[0]
    },

    # Rejected
    {
        'trigger': 'Input',
        'source': states[2],
        'dest': states[2]
    },

    # Rejected while matching with subset
    {
        'trigger': 'Input',
        'source': states[3],
        'dest': states[2]
    },

    # Accepted
    {
        'trigger': 'Input',
        'source': states[4],
        'dest': states[2]
    },

    # Accepted in <num>
    {
        'trigger': 'Input numbers',
        'source': states[5],
        'dest': states[5]
    },
    {
        'trigger': 'Input others',
        'source': states[5],
        'dest': states[2]
    },

    # Accepted in <any>
    {
        'trigger': 'Input',
        'source': states[6],
        'dest': states[6]
    }
]

model = Model()
machine = GraphMachine(
        model=model,
        states=states,
        transitions=transitions,
        initial=states[0],
        title='LoggerParser State Transition Diagram')

graph = model.get_graph()
graph.draw('logger_parser_state_transition_diagram.png', prog='dot')
