/**
********************************************************************************
*
*   @file       Sinogram.h
*
*   @brief      Class to reconstruct images from a sinogram.
*
*   @version    1.0
*
*   @date       14/09/2016
*
*   @author     Franck Vidal
*
*
********************************************************************************
*/


//******************************************************************************
//  Define
//******************************************************************************


//******************************************************************************
//  Include
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


template<typename T> Sinogram<T>::Sinogram():
        Image<T>()
{}


template<typename T> Sinogram<T>::Sinogram(const Image<T> anImage):
        Image<T>(anImage)
{}


template<typename T> Sinogram<T>::Sinogram(unsigned int aWidth,
            unsigned int aHeight,
            unsigned int aNumberOfSlices):
        Image<T>(aWidth, aHeight, aNumberOfSlices)
{}


//------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::ART(double aFirstAngle,
                                               double anAngleStep,
                                               double aLastAngle,
                                               unsigned int aNumberOfIterations,
                                               const char* aDirectory) const
//------------------------------------------------------------------------------
{
    unsigned int image_width(2 * floor (this->m_width / (2.0 * std::sqrt(2.0))));

    // Start with a homogenous image
    Image<T> new_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));
    Image<T> old_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));

    double old_ncc(0.0);

    unsigned int iteration(1);
    bool stop(false);
    while (!stop)
    {
        old_estimated_image = new_estimated_image;
    
        // Process every angle
        double angle(aFirstAngle);
        for (unsigned int angle_id(0); angle_id < this->m_height; ++angle_id, angle += anAngleStep)
        {
            // Compute the projections
            Image<T> estimated_projection(new_estimated_image.radonTransform(angle, 0.0, angle).setCanvasSize(this->m_width, 1, this->m_number_of_slices));
            //double sum_estimated(estimated_projection.getSum());

            // Real image
            Image<T> real_projection(this->getROI(0, angle_id, 0, this->m_width, 1, this->m_number_of_slices));

            // Compute the error
            Sinogram<T> error_in_projections(real_projection - estimated_projection);
        
            // Correct for errors
            Image<T> error_back_projection(error_in_projections.simpleBackProjection(angle, 0.0, angle));
        
            new_estimated_image += error_back_projection.setCanvasSize(image_width, image_width, this->m_number_of_slices);

            T* p_temp(new_estimated_image.m_p_image);
            for (unsigned int i(0); i < new_estimated_image.m_width * new_estimated_image.m_height * new_estimated_image.m_number_of_slices; ++i, ++p_temp)
            {
                *p_temp = std::max(0.0f, *p_temp);
            }

            if (aDirectory)
            {
                std::stringstream filename;
                filename << aDirectory << "/iteration_" << iteration << "_" << angle_id << ".pgm";
                (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices).normalised() * 255).savePGM(filename.str());
            }
        }

        // Compute the projection
        Image<T> estimated_projections(new_estimated_image.radonTransform(aFirstAngle, anAngleStep, aLastAngle).setCanvasSize(this->m_width, this->m_height, this->m_number_of_slices));

        // Stopping criteria
        double ncc_projections(100.0 * this->computeNCC(estimated_projections));
        double ncc_reconstructions(100.0 * new_estimated_image.computeNCC(old_estimated_image));

        if ((ncc_projections  > 99.99 || ncc_reconstructions > 99.99) && std::abs(old_ncc - ncc_reconstructions) < EPSILON)
        {
            stop = true;
        }
        else if (aNumberOfIterations == iteration)
        {
            stop = true;
        }

        old_ncc = ncc_reconstructions;

        iteration++;

    }
    
    return (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices));
}


//-------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::MART(double aFirstAngle,
                                                double anAngleStep,
                                                double aLastAngle,
                                                unsigned int aNumberOfIterations,
                                                const char* aDirectory) const
//-------------------------------------------------------------------------------
{
    unsigned int image_width(2 * floor (this->m_width / (2.0 * std::sqrt(2.0))));

    // Start with a homogenous image
    Image<T> new_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));
    Image<T> old_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));

    double old_ncc(0.0);

    unsigned int iteration(1);
    bool stop(false);
    while (!stop)
    {
        old_estimated_image = new_estimated_image;
    
        // Process every angle
        double angle(aFirstAngle);
        for (unsigned int angle_id(0); angle_id < this->m_height; ++angle_id, angle += anAngleStep)
        {
            // Compute the projections
            Image<T> estimated_projection(new_estimated_image.radonTransform(angle, 0.0, angle).setCanvasSize(this->m_width, 1, this->m_number_of_slices));
            //double sum_estimated(estimated_projection.getSum());

            // Real image
            Image<T> real_projection(this->getROI(0, angle_id, 0, this->m_width, 1, this->m_number_of_slices));

            // Compute the error
            Sinogram<T> error_in_projections(real_projection - estimated_projection);
        
            // Correct for errors
            Image<T> error_back_projection(error_in_projections.simpleBackProjection(angle, 0.0, angle));
        
            new_estimated_image *= error_back_projection.setCanvasSize(image_width, image_width, this->m_number_of_slices);

            T* p_temp(new_estimated_image.m_p_image);
            for (unsigned int i(0); i < new_estimated_image.m_width * new_estimated_image.m_height * new_estimated_image.m_number_of_slices; ++i, ++p_temp)
            {
                *p_temp = std::max(0.0f, *p_temp);
            }

            if (aDirectory)
            {
                std::stringstream filename;
                filename << aDirectory << "/iteration_" << iteration << "_" << angle_id << ".pgm";
                (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices).normalised() * 255).savePGM(filename.str());
            }
        }

        // Compute the projection
        Image<T> estimated_projections(new_estimated_image.radonTransform(aFirstAngle, anAngleStep, aLastAngle).setCanvasSize(this->m_width, this->m_height, this->m_number_of_slices));

        // Stopping criteria
        double ncc_projections(100.0 * this->computeNCC(estimated_projections));
        double ncc_reconstructions(100.0 * new_estimated_image.computeNCC(old_estimated_image));

        if ((ncc_projections  > 99.99 || ncc_reconstructions > 99.99) && std::abs(old_ncc - ncc_reconstructions) < EPSILON)
        {
            stop = true;
        }
        else if (aNumberOfIterations == iteration)
        {
            stop = true;
        }

        old_ncc = ncc_reconstructions;

        iteration++;

    }
    
    return (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices));
}


//-------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::SIRT(double aFirstAngle,
                                                double anAngleStep,
                                                double aLastAngle,
                                                unsigned int aNumberOfIterations,
                                                const char* aDirectory) const
//-------------------------------------------------------------------------------
{
    unsigned int image_width(2 * floor (this->m_width / (2.0 * std::sqrt(2.0))));

    // Start with a homogenous image
    Image<T> new_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));
    Image<T> old_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));

    std::cout << this->getSum() << "\t" << new_estimated_image.getSum() << std::endl;
    double old_ncc(0.0);

    unsigned int iteration(1);
    bool stop(false);
    while (!stop)
    {
        old_estimated_image = new_estimated_image;

        Image<T> estimated_projection(new_estimated_image.radonTransform(aFirstAngle, anAngleStep, aLastAngle));

        // Compute the error
        Sinogram<T> error_in_projections(*this - estimated_projection);

        this->saveASCII("real_projection.mat");
        estimated_projection.saveASCII("estimated_projection.mat");
        error_in_projections.saveASCII("error_in_projections.mat");

        Image<T> error_back_projection(error_in_projections.simpleBackProjection(aFirstAngle, anAngleStep, aLastAngle));
        error_back_projection.saveASCII("error_back_projection.mat");

        // Correct for errors
        new_estimated_image += error_back_projection.setCanvasSize(image_width, image_width, this->m_number_of_slices);
        new_estimated_image.saveASCII("new_estimated_image.mat");
        old_estimated_image.saveASCII("old_estimated_image.mat");

        T* p_temp(new_estimated_image.m_p_image);
        for (unsigned int i(0); i < new_estimated_image.m_width * new_estimated_image.m_height * new_estimated_image.m_number_of_slices; ++i, ++p_temp)
        {
        	*p_temp = std::max(0.0f, *p_temp);
        }

        if (aDirectory)
        {
            std::stringstream filename;
            filename << aDirectory << "/iteration_" << iteration << ".pgm";
            (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices).normalised() * 255).savePGM(filename.str());
        }

        // Compute the projection
        Image<T> estimated_projections(new_estimated_image.radonTransform(aFirstAngle, anAngleStep, aLastAngle).setCanvasSize(this->m_width, this->m_height, this->m_number_of_slices));
        estimated_projections.saveASCII("simulated_projection.mat");

        // Stopping criteria
        double ncc_projections(100.0 * this->computeNCC(estimated_projections));
        double ncc_reconstructions(100.0 * new_estimated_image.computeNCC(old_estimated_image));

        if ((ncc_projections  > 99.99 || ncc_reconstructions > 99.99) && std::abs(old_ncc - ncc_reconstructions) < EPSILON)
        {
            stop = true;
        }
        else if (aNumberOfIterations == iteration)
        {
            stop = true;
        }

        old_ncc = ncc_reconstructions;

        iteration++;

    }

    return (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices));
}


//-------------------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::simpleBackProjection(double aFirstAngle,
                                                                double anAngleStep,
                                                                double aLastAngle,
                                                                const char* aDirectory) const
//-------------------------------------------------------------------------------------------
{
	Image<T> sinogram(this->resize(this->m_width * 2, this->m_height, this->m_number_of_slices));
	sinogram.saveASCII("resized.mat");
	unsigned int image_width(2 * floor (sinogram.m_width / (2.0 * std::sqrt(2.0))));

    Image<T> reconstruction(sinogram.m_width, sinogram.m_width, sinogram.m_number_of_slices);

    double angle(aFirstAngle);
    for (unsigned int angle_id(0); angle_id < this->m_height; ++angle_id, angle += anAngleStep)
    {
        if (angle_id == 130)
        {
            int itr=0;
        }
        Image<T> back_projection(sinogram.m_width, sinogram.m_width, sinogram.m_number_of_slices);

        for (int k(0); k < reconstruction.m_number_of_slices; ++k)
        {
            for (int j(0); j < reconstruction.m_height; ++j)
            {
                for (int i(0); i < reconstruction.m_width; ++i)
                {
                    back_projection(i, j, k) += sinogram.getPixel(i, angle_id, k);
                }
            }
        }

        reconstruction += back_projection.rotate(-angle) / reconstruction.m_height;

        if (aDirectory)
        {
            std::stringstream filename;
            filename << aDirectory << "/angle_" << angle_id << ".pgm";
            (reconstruction.setCanvasSize(image_width, image_width, reconstruction.m_number_of_slices).resize(image_width / 2, image_width / 2, reconstruction.m_number_of_slices).normalised() * 255).savePGM(filename.str());
        }
    }

    //reconstruction = reconstruction.rotate(-angle);

    reconstruction = reconstruction.setCanvasSize(image_width, image_width, this->m_number_of_slices);
    reconstruction *= M_PI / (/*2.0 * */ this->m_height);

    return (reconstruction.resize(image_width / 2, image_width / 2, this->m_number_of_slices));
}


//---------------------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::filteredBackProjection(double aFirstAngle,
                                                                  double anAngleStep,
                                                                  double aLastAngle,
                                                                  const char* aFilterType,
                                                                  float aScalingFactor,
                                                                  const char* aDirectory) const
//---------------------------------------------------------------------------------------------
{
    typedef  FFT<T>   FAST_FOURIER_TRANSFORM;

	// Compute the FFTs
	std::vector<FAST_FOURIER_TRANSFORM> p_fft_set;
	for (unsigned int j(0); j < this->m_height; ++j)
	{
		p_fft_set.push_back(this->getROI(0, j, 0, this->m_width, 1, this->m_number_of_slices).getFFT());
	}

	// Compute the rho filter
	for (typename std::vector<FAST_FOURIER_TRANSFORM>::iterator ite(p_fft_set.begin());
			ite != p_fft_set.end();
			++ite)
	{
	    *ite = ite->rhoFilter(aFilterType, aScalingFactor);
	}

	// Compute the iFFTs
	std::vector<Image<T> > p_ifft_set(this->m_height);
	for (unsigned int j(0); j < this->m_height; ++j)
	{
		p_ifft_set[j] = p_fft_set[j].getInverseFFT();
	}

	// Store the results
    Sinogram<T> filtered_sinogram(this->m_width, this->m_height);

    for (unsigned int k(0); k < this->m_number_of_slices; ++k)
    {
        for (unsigned int j(0); j < this->m_height; ++j)
        {
            for (unsigned int i(0); i < this->m_height; ++i)
            {
                filtered_sinogram(i, j, k) = p_ifft_set[j](i, 0, k);
            }
        }
    }

	//std::cout << getAverage() << "  " << filtered_sinogram.getAverage() << std::endl;
	//filtered_sinogram *= filtered_sinogram.getAverage() / getAverage();
	filtered_sinogram.saveASCII("filtered_sinogram.mat");
    return (filtered_sinogram.simpleBackProjection(aFirstAngle, anAngleStep, aLastAngle, aDirectory));
}


//---------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::ART(double aFirstAngle,
                                               double anAngleStep, 
                                               double aLastAngle, 
                                               unsigned int aNumberOfIterations, 
                                               const std::string& aDirectory) const
//---------------------------------------------------------------------------------
{
    return (ART(aFirstAngle, anAngleStep, aLastAngle, aNumberOfIterations, aDirectory.data()));
}


//----------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::MART(double aFirstAngle,
                                                double anAngleStep,
                                                double aLastAngle,
                                                unsigned int aNumberOfIterations,
                                                const std::string& aDirectory) const
//----------------------------------------------------------------------------------
{
    return (MART(aFirstAngle, anAngleStep, aLastAngle, aNumberOfIterations, aDirectory.data()));
}


//----------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::SIRT(double aFirstAngle,
                                                double anAngleStep,
                                                double aLastAngle,
                                                unsigned int aNumberOfIterations,
                                                const std::string& aDirectory) const
//----------------------------------------------------------------------------------
{
    return (SIRT(aFirstAngle, anAngleStep, aLastAngle, aNumberOfIterations, aDirectory.data()));
}


//--------------------------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::simpleBackProjection(double aFirstAngle,
                                                                double anAngleStep,
                                                                double aLastAngle,
                                                                const std::string& aDirectory) const
//--------------------------------------------------------------------------------------------------
{
    return (simpleBackProjection(aFirstAngle, anAngleStep, aLastAngle, aDirectory.data()));
}


//----------------------------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::filteredBackProjection(double aFirstAngle,
                                                                  double anAngleStep,
                                                                  double aLastAngle,
                                                                  const std::string& aFilterType,
                                                                  float aScalingFactor,
                                                                  const std::string& aDirectory) const
//----------------------------------------------------------------------------------------------------
{
    return (filteredBackProjection(aFirstAngle, anAngleStep, aLastAngle, aFilterType.data(), aScalingFactor, aDirectory.data()));
}


} // namespace gVirtualXRay
