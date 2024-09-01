#include <miniocpp/client.h>

std::string GetObjectName(const std::string& filename);

int main(int argc, char** argv) {
  // create S3 base URL
  minio::s3::BaseUrl base_url("play.min.io");

  // create credential provider
  minio::creds::StaticProvider provider("minioadmin", "minioadmin");

  // Create S3 client
  minio::s3::Client client(base_url, &provider);

  std::string bucket_name = "vadimbucket";

  // check 'vadimbucket' existing or not
  bool exist;
  {
    minio::s3::BucketExistsArgs args;
    args.bucket = bucket_name;

    minio::s3::BucketExistsResponse resp = client.BucketExists(args);
    if (!resp) {
      std::cout << "unable to do bucket existance check; " << resp.Error().String()
        << std::endl;
      return EXIT_FAILURE;
    }
    exist = resp.exist;
  }

  // make 'vadimbucket' if not exist
  if (!exist) {
    minio::s3::MakeBucketArgs args;
    args.bucket = bucket_name;

    minio::s3::MakeBucketResponse resp = client.MakeBucket(args);
    if (!resp) {
      std::cout << "unable to create bucket; " << resp.Error() << std::endl;
      return EXIT_FAILURE;
    }
  }

  // upload new file or not
  {
    std::cout << "Upload new file to a bucket? (Y/N)";
    char c;
    std::cin >> c;
    if (c == 'Y' || c == 'y') {
      std::cout << "Input file path: ";
      std::string filename;
      std::cin >> filename;

      minio::s3::UploadObjectArgs args;
      args.bucket = bucket_name;
      args.object = GetObjectName(filename);
      args.filename = filename;

      minio::s3::UploadObjectResponse resp = client.UploadObject(args);
      if (!resp) {
        std::cout << "unable to upload object; " << resp.Error() << std::endl;
        return EXIT_FAILURE;
      }

      std::cout << filename << " is successfully uploaded as object '"
        << args.object << "' to bucket " << args.bucket << std::endl;
    }
  }

  // list objects
  {
    // create list objects arguments
    minio::s3::ListObjectsArgs args;
    args.bucket = bucket_name;

    // call list objects
    minio::s3::ListObjectsResult result = client.ListObjects(args);
    for (; result; result++) {
      minio::s3::Item item = *result;
      if (item) {
        std::cout << "Name: " << item.name << std::endl;
        std::cout << "Version ID: " << item.version_id << std::endl;
        std::cout << "ETag: " << item.etag << std::endl;
        std::cout << "Size: " << item.size << std::endl;
        std::cout << "Last Modified: " << item.last_modified << std::endl;
        std::cout << "Delete Marker: "
                  << minio::utils::BoolToString(item.is_delete_marker)
                  << std::endl;
        std::cout << "User Metadata: " << std::endl;
        for (auto& [key, value] : item.user_metadata) {
          std::cout << "  " << key << ": " << value << std::endl;
        }
        std::cout << "Owner ID: " << item.owner_id << std::endl;
        std::cout << "Owner Name: " << item.owner_name << std::endl;
        std::cout << "Storage Class: " << item.storage_class << std::endl;
        std::cout << "Is Latest: " << minio::utils::BoolToString(item.is_latest)
                  << std::endl;
        std::cout << "Is Prefix: " << minio::utils::BoolToString(item.is_prefix)
                  << std::endl;
        std::cout << "---" << std::endl;
      } else {
        std::cout << "unable to listobjects; " << item.Error().String()
                  << std::endl;
        break;
      }
    }
  }


  return EXIT_SUCCESS;
}

std::string GetObjectName(const std::string& filename) {
  auto const pos = filename.find_last_of("/");
  if (pos == filename.npos) {
    return filename;
  }

  return filename.substr(pos + 1);
}
