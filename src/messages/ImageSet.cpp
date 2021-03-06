#include "messages/ImageSet.hpp"

#include "singletons/Settings.hpp"

namespace chatterino {

ImageSet::ImageSet()
    : imageX1_(Image::getEmpty())
    , imageX2_(Image::getEmpty())
    , imageX3_(Image::getEmpty())
{
}

ImageSet::ImageSet(const ImagePtr &image1, const ImagePtr &image2,
                   const ImagePtr &image3)
    : imageX1_(image1)
    , imageX2_(image2)
    , imageX3_(image3)
{
}

ImageSet::ImageSet(const Url &image1, const Url &image2, const Url &image3)
    : imageX1_(Image::fromUrl(image1, 1))
    , imageX2_(image2.string.isEmpty() ? Image::getEmpty()
                                       : Image::fromUrl(image2, 0.5))
    , imageX3_(image3.string.isEmpty() ? Image::getEmpty()
                                       : Image::fromUrl(image3, 0.25))
{
}

void ImageSet::setImage1(const ImagePtr &image)
{
    this->imageX1_ = image;
}

void ImageSet::setImage2(const ImagePtr &image)
{
    this->imageX2_ = image;
}

void ImageSet::setImage3(const ImagePtr &image)
{
    this->imageX3_ = image;
}

const ImagePtr &ImageSet::getImage1() const
{
    return this->imageX1_;
}

const ImagePtr &ImageSet::getImage2() const
{
    return this->imageX2_;
}

const ImagePtr &ImageSet::getImage3() const
{
    return this->imageX3_;
}

const ImagePtr &ImageSet::getImage(float scale) const
{
    // get best image based on scale
    auto &&result = [&]() -> const std::shared_ptr<Image> & {
        scale *= getSettings()->emoteScale;

        int quality = 1;

        if (scale > 2.001f)
            quality = 3;
        else if (scale > 1.001f)
            quality = 2;

        if (!this->imageX3_->isEmpty() && quality == 3)
        {
            return this->imageX3_;
        }

        if (!this->imageX2_->isEmpty() && quality == 2)
        {
            return this->imageX2_;
        }

        return this->imageX1_;
    }();

    // prefer other image if selected image is not loaded yet
    if (result->loaded())
        return result;
    else if (this->imageX1_->loaded())
        return this->imageX1_;
    else if (this->imageX2_ && !this->imageX2_->isEmpty() &&
             this->imageX2_->loaded())
        return this->imageX2_;
    else if (this->imageX3_ && !this->imageX3_->isEmpty() &&
             this->imageX3_->loaded())
        return this->imageX3_;
    else
        return result;
}

bool ImageSet::operator==(const ImageSet &other) const
{
    return std::tie(this->imageX1_, this->imageX2_, this->imageX3_) ==
           std::tie(other.imageX1_, other.imageX2_, other.imageX3_);
}

bool ImageSet::operator!=(const ImageSet &other) const
{
    return !this->operator==(other);
}

}  // namespace chatterino
