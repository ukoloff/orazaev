DownloadLoansData <- function() {
    url <- "https://spark-public.s3.amazonaws.com/dataanalysis/loansData.rda"
    destination <- "./data/loansData.rda"
    dateFileDestination <- "./data/download_date.txt"
    
    download.file(url, destination, method="curl")

    downloadDateFile = file(dateFileDestination, "wb")
    write(date(), downloadDateFile)
    close(downloadDateFile)
}
