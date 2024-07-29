#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <sys/stat.h>

/*
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/LogMacros.h>
*/

#include <iostream>

bool FindMaxObjectInBucket(const Aws::S3::S3Client& s3Client, const Aws::String& bucketName) {
    Aws::S3::Model::ListObjectsV2Request request;
    request.WithBucket(bucketName);

    auto outcome = s3Client.ListObjectsV2(request);

    if (!outcome.IsSuccess())
    {
        std::cerr << "Error: listObjects: " <<
            outcome.GetError().GetMessage() << std::endl;
        return false;
    }
    
    Aws::Vector<Aws::S3::Model::Object> objects = outcome.GetResult().GetContents();

    if (objects.empty())
    {
        std::cerr << "No objects found in the bucket '" << bucketName << "'" << std::endl;
        return false;
    }

    size_t maxSize = 0;
    Aws::S3::Model::Object maxObject;
    for (const auto& object : objects)
    {
        if (object.GetSize() > maxSize)
        {
            maxSize = object.GetSize();
            maxObject = object;
        }
    }

    std::cout << maxObject.GetKey() << " " << maxObject.GetSize() << std::endl;

    return true;
}

int main(int argc, char* argv[])
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        Aws::Client::ClientConfiguration clientConfig;

        clientConfig.endpointOverride = "storage.yandexcloud.net";
        clientConfig.scheme = Aws::Http::Scheme::HTTPS;
        clientConfig.region = "ru-central1";

        /*
		Aws::Utils::Logging::InitializeAWSLogging(
			Aws::MakeShared<Aws::Utils::Logging::ConsoleLogSystem>(
				"RunUnitTests", Aws::Utils::Logging::LogLevel::Trace));
        */

        Aws::S3::S3Client s3_client(clientConfig);

        Aws::String bucket_name = "vadimbucket1";
        FindMaxObjectInBucket(s3_client, bucket_name);

        /*
		Aws::Utils::Logging::ShutdownAWSLogging();
        */
    }
    Aws::ShutdownAPI(options);

    return 0;
}
