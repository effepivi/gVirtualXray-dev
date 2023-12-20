import json # Save the JSON file
from gvxrPython3 import gvxr # Simulate X-ray images


def saveJSON(fname: str):
    params = {}
    
#      Not implemented as yet
    # params["WindowSize"] = gvxr.getWindowSize()
    params["WindowSize"] = [500, 500]
    
    params["Source"] = {}
    
    position = gvxr.getSourcePosition("mm")
    params["Source"]["Position"] = [position[0], position[1], position[2], "mm"]
    
    params["Source"]["Shape"] = gvxr.getSourceShape();
    if gvxr.getSourceShape() == "FOCALSPOT":
        raise IOError('Exporter for gvxr.getSourceShape() == "FOCALSPOT" is not implemented')
        
    params["Source"]["Beam"] = []
    for energy, count in zip(energy_bins, photon_counts):
        params["Source"]["Beam"].append({
            "Energy": energy,
            "PhotonCount": count,
            "Unit": "keV"
        })
    
    params["Detector"] = {}
    
    position = gvxr.getDetectorPosition("mm")
    params["Detector"]["Position"] = [position[0], position[1], position[2], "mm"]
    
    v = gvxr.getDetectorUpVector()
    params["Detector"]["UpVector"] = [v[0], v[1], v[2]]

    u = gvxr.getDetectorRightVector()
    params["Detector"]["RightVector"] = [u[0], u[1], u[2]]

    cols, rows = gvxr.getDetectorNumberOfPixels()
    params["Detector"]["NumberOfPixels"] = [cols, rows]
    
    cols, rows = gvxr.getDetectorSize("mm")
    params["Detector"]["Size"] = [cols, rows, "mm"]

    response = gvxr.getLSF();
    if len(response) > 0:
        params["Detector"]["LSF"] = response
    
    response = gvxr.getEnergyResponse("keV");
    if len(response) > 0:
        raise IOError('Exporter for gvxr.getEnergyResponse("keV") is not implemented')
        # params["Detector"]["Energy response"] =
    
    
    params["Samples"] = []
    
    for i in range(gvxr.getNumberOfChildren("root")):
        label = gvxr.getChildLabel("root", i);
        
        params["Samples"].append({})
        params["Samples"][-1]["Label"] = label
        params["Samples"][-1]["Path"] = gvxr.getMeshFilename(label)
        params["Samples"][-1]["Unit"] = gvxr.getMeshUnitOfLength(label)
                
        material_label = gvxr.getMaterialLabel(label)

        if "Element" in material_label or \
            "Compound" in material_label or \
            "Mixture" in material_label:

            material_type = material_label.split(": ")[0]
            material_properties = material_label.split(": ")[1]
        
            params["Samples"][-1]["Material"] = [material_type, material_properties]
            params["Samples"][-1]["Density"] = gvxr.getDensity(label)
        else:
            raise IOError('Exporter for Material ' + material_label + ' is not implemented')

        params["Samples"][-1]["Matrix"] = gvxr.getLocalTransformationMatrix(label)

            

    # Convert and write JSON object to file
    with open(fname, "w") as outfile: 
        json.dump(params, outfile)
        
saveJSON("test.json")
