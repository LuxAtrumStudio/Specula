# Examples

Here are a number of examples to take inspiration from, and to see how the
mechanics of constructing a scene works.

## Diffuse Sphere :id=diffusesphere

This is the basic scene, containing two diffuse spheres in a box, and a light
source for the celling.

```lua
camera:eye(2.78, 2.73, -8.00)
camera:direction(0.0, 0.0, 1.0)
camera:up(0.0, 1.0, 0.0)

light_mat = Material.new(1.0, 1.0, 1.0):set_emission(1.0)

white_mat = Material.new(1.0, 1.0, 1.0):set_type(Material.Type.DIFFUSE)
green_mat = Material.new(0.0, 1.0, 0.0):set_type(Material.Type.DIFFUSE)
red_mat = Material.new(1.0, 0.0, 0.0):set_type(Material.Type.DIFFUSE)

floor = Plane.new(0.0, 1.0, 0.0, 0.0):set_material(white_mat)
ceiling = Plane.new(0.0, -1.0, 0.0, 5.5):set_material(light_mat)
back_wall = Plane.new(0.0, 0.0, -1.0, 5.592):set_material(white_mat)
right_wall = Plane.new(1.0, 0.0, 0.0, 0.0):set_material(green_mat)
left_wall = Plane.new(-1.0, 0.0, 0.0, 5.528):set_material(red_mat)
front_wall = Plane.new(0.0, 0.0, 1.0, -8.1):set_material(white_mat)

large = Sphere.new(2.0):translate(3.528, 2.0, 3.592):set_material(white_mat)
small = Sphere.new(0.8):translate(1.0, 1.0, 0.5):set_material(white_mat)
```

This will produce the output shown below.

![Diffuse Sphere](diffuse.png)