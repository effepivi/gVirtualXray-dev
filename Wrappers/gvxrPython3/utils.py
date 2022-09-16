import numpy as np

from skimage.filters import gaussian # Implementing the image sharpening filter
from stl import mesh

try:
    import k3d
    from k3d.factory import *
    has_k3d = True
except:
    print("K3D is not install, you won't be able to visualise the 3D scene using k3D")
    has_k3d = False

try:
    import PIL
    has_PIL = True
except:
    print("PIL is not install, you won't be able to visualise the X-ray image in the 3D scene using k3D")
    has_PIL = False

import math
from gvxrPython3 import gvxr # Simulate X-ray images
from gvxrPython3 import json2gvxr


def standardisation(img):
    return (img - img.mean()) / img.std()

def logImage(xray_image: np.array, min_val: float, max_val: float) -> np.array:
    
    log_epsilon = 1.0e-9

    shift_filter = -math.log(min_val + log_epsilon)
    
    if min_val != max_val:
        scale_filter = 1.0 / (math.log(max_val + log_epsilon) - math.log(min_val + log_epsilon))
    else:
        scale_filter = 1.0

    corrected_image = np.log(xray_image + log_epsilon)

    return (corrected_image + shift_filter) * scale_filter

def applyLogScaleAndNegative(image: np.array) -> np.array:
    temp = logImage(image, image.min(), image.max())
    return 1.0 - temp

def sharpen(image, ksize, alpha):
    details = image - gaussian(image, ksize)    

    return image + alpha * details
 
def rgb2k3d(R, G, B):
    k3d_color = 0;
    k3d_color |= (R & 255) << 16;
    k3d_color |= (G & 255) << 8;
    k3d_color |= (B & 255);
    return k3d_color
    
def visualize(flip=False, use_log=False, use_negative=False, sharpen_ksize=1, sharpen_alpha=0.0):
    
    plot = None
    
    if has_k3d:
        up_x,up_y, up_z = gvxr.getDetectorUpVector()
        src_x_pos, src_y_pos, src_z_pos = gvxr.getSourcePosition("mm")
        det_x_pos, det_y_pos, det_z_pos = gvxr.getDetectorPosition("mm")
        sdd = math.sqrt(math.pow(src_x_pos - det_x_pos, 2) + math.pow(src_y_pos - det_y_pos, 2) + math.pow(src_z_pos - det_z_pos, 2))

        w, h = gvxr.getDetectorSize("mm")
        half_w = w / 2
        half_h = h / 2

        half_view_angle_rad = math.atan(half_w / sdd)
        half_view_angle_deg = half_view_angle_rad * 180 / math.pi
        fov = 2.0 * half_view_angle_deg

        view_vec_x = src_x_pos - det_x_pos
        view_vec_y = src_y_pos - det_y_pos
        view_vec_z = src_z_pos - det_z_pos
        
        camera = [
            src_x_pos + view_vec_x, src_y_pos + view_vec_y, src_z_pos + view_vec_z, # position of the camera in xyz space
            det_x_pos, det_y_pos, det_z_pos, # the point where camera is currently looking at
            -up_x, -up_y, -up_z  # orientation (up direction), this vector cannot be [0,0,0])
        ]

        plot = k3d.plot()
        plot.background_color = 0xffffff


        

        sphere = points(
                positions=[src_x_pos, src_y_pos, src_z_pos],
                color=rgb2k3d(255, 0, 0),
                point_size=10,
                shader="mesh"
        )

        plot += sphere 

        # add detector
        # Define the 4 vertices of the quad
        p0 = [det_x_pos - half_w, det_y_pos, det_z_pos - half_h]
        p1 = [det_x_pos + half_w, det_y_pos, det_z_pos - half_h]
        p2 = [det_x_pos + half_w, det_y_pos, det_z_pos + half_h]
        p3 = [det_x_pos - half_w, det_y_pos, det_z_pos + half_h]

        vertices = np.array([p0, p1, p2, p3])

        # Define the 12 triangles composing the cube
        faces = np.array([\
            [0,1,2],
            [0,2,3]])

        # Create the mesh
        # quad = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
        # for i, f in enumerate(faces):
        #     for j in range(3):
        #         quad.vectors[i][j] = vertices[f[j],:]

        # # Write the mesh to file "detector.stl"
        # fname = "detector.stl"
        # quad.save(fname)

        # Y-Z flip
        model_matrix = [
            w, 0.0, 0.0, det_x_pos,
            0.0, -h, 0.0, det_z_pos,
            0.0, 0.0, 1, -det_y_pos,
            0.0, 0.0, 0.0, 1.0
        ]
        
        if has_PIL:
            xray_image = np.flip(gvxr.computeXRayImage(), 0)
            total_energy_in_MeV = gvxr.getTotalEnergyWithDetectorResponse()
            white = np.ones(xray_image.shape) * total_energy_in_MeV
            dark = np.zeros(xray_image.shape)
            xray_image_flat = ((xray_image - dark) / (white - dark)).astype(np.single)
            
            if flip:
                xray_image_flat = np.flip(xray_image - dark)
            
            if use_log:
                xray_image_flat = logImage(xray_image_flat, 0.0, 1.0)
                xray_image_flat = (xray_image_flat - xray_image_flat.min()) / (xray_image_flat.max() - xray_image_flat.min())
                
            if use_negative:
                xray_image_flat = 1.0 - xray_image_flat

            xray_image_flat = sharpen(xray_image_flat, sharpen_ksize, sharpen_alpha)
            xray_image_flat[xray_image_flat<0] = 0
            xray_image_flat[xray_image_flat>1] = 1
            
            # save a image using extension
            pil_img = PIL.Image.fromarray((255 * xray_image_flat).astype(np.uint8))

            if pil_img.mode != 'RGB':
                pil_img = pil_img.convert('L')

            pil_img.save("temp_texture.png")        

            texture = k3d.texture(open('temp_texture.png', 'br').read(), 'png', 
                                  rotation=[math.radians(90),1,0,0], 
                                  model_matrix = model_matrix,
                                  name='Detector')

            plot += texture 
        
        
        
        nodes_to_be_processed = ["root"]
        
        while len(nodes_to_be_processed) > 0:
            
            # Get the last node
            node = nodes_to_be_processed[-1]
            nodes_to_be_processed.pop()
            
            for i in range(gvxr.getNumberOfChildren(node)):
                nodes_to_be_processed.append(gvxr.getChildLabel(node, i))
        
            # Get the triangles
            vertex_set = gvxr.getVertexSet(node)
            index_set = gvxr.getIndexSet(node)
            
            if len(index_set) == 0:
                index_set = range(int(len(vertex_set) / 3))
            # Get the colour
            r, g, b, a = gvxr.getAmbientColour(node)
            R = math.floor(255*r)
            G = math.floor(255*g)
            B = math.floor(255*b)

            if node.upper() == "MUSCLE" or node.upper() == "SKIN":
                opacity = 0.4
            else:
                opacity = 1

            # Create the geometry
            geometry = k3d.mesh(vertex_set,
                                  index_set,
                                  color=rgb2k3d(R, G, B), 
                                  wireframe=False, 
                                  flat_shading=False,
                                  name=node,
                                  opacity=opacity)

            # Add the geometry to the plot
            plot += geometry   

        # Display the plot
        plot.display()
        
        plot.camera_auto_fit=False
        plot.grid_visible=False
        plot.camera = camera
        plot.camera_fov = fov

    else:
        print("K3D was not found.")
        
    return plot
    
def visualise(flip=False, use_log=False, use_negative=False, sharpen_ksize=1, sharpen_alpha=0.0):
    visualize(flip, use_log, use_negative, sharpen_ksize, sharpen_alpha)
        
