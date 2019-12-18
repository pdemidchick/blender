import bpy
import random
from bpy.props import *
from .. base import FunctionNode
from .. node_builder import NodeBuilder


class PerlinNoiseNode(bpy.types.Node, FunctionNode):
    bl_idname = "fn_PerlinNoiseNode"
    bl_label = "Perlin Noise"

    def declaration(self, builder: NodeBuilder):
        builder.fixed_input("position", "Position", "Vector")
        builder.fixed_input("amplitude", "Amplitude", "Float", default=1)
        builder.fixed_input("scale", "Scale", "Float", default=1)
        builder.fixed_output("noise_1d", "Noise", "Float")
        builder.fixed_output("noise_3d", "Noise", "Vector")


class RandomFloatNode(bpy.types.Node, FunctionNode):
    bl_idname = "fn_RandomFloatNode"
    bl_label = "Random Float"

    node_seed: IntProperty(
        name="Node Seed",
    )

    def init_props(self):
        self.node_seed = new_node_seed()

    def declaration(self, builder: NodeBuilder):
        builder.fixed_input("min", "Min", "Float", default=0)
        builder.fixed_input("max", "Max", "Float", default=1)
        builder.fixed_input("seed", "Seed", "Integer")
        builder.fixed_output("value", "Value", "Float")

    def draw_advanced(self, layout):
        layout.prop(self, "node_seed")

    def duplicate(self, src_node):
        self.node_seed = new_node_seed()


class RandomFloatsNode(bpy.types.Node, FunctionNode):
    bl_idname = "fn_RandomFloatsNode"
    bl_label = "Random Floats"

    node_seed: IntProperty(
        name="Node Seed",
    )

    def init_props(self):
        self.node_seed = new_node_seed()

    def declaration(self, builder: NodeBuilder):
        builder.fixed_input("amount", "Amount", "Integer", default=10)
        builder.fixed_input("min", "Min", "Float")
        builder.fixed_input("max", "Max", "Float", default=1)
        builder.fixed_input("seed", "Seed", "Integer")
        builder.fixed_output("values", "Values", "Float List")

    def draw_advanced(self, layout):
        layout.prop(self, "node_seed")

    def duplicate(self, src_node):
        self.node_seed = new_node_seed()


class RandomVectorNode(bpy.types.Node, FunctionNode):
    bl_idname = "fn_RandomVectorNode"
    bl_label = "Random Vector"

    node_seed: IntProperty(
        name="Node Seed",
    )

    def init_props(self):
        self.node_seed = new_node_seed()

    def declaration(self, builder: NodeBuilder):
        builder.fixed_input("amplitude", "Amplitude", "Vector", default=(1, 1, 1))
        builder.fixed_input("seed", "Seed", "Integer")
        builder.fixed_output("vector", "Vector", "Vector")

    def draw_advanced(self, layout):
        layout.prop(self, "node_seed")

    def duplicate(self, src_node):
        self.node_seed = new_node_seed()


def new_node_seed():
    return random.randint(0, 10000)
