double mean_vector(const vector<double> data)
{
        double size = 1.0*data.size();
        double sum = 0;
        for(int i = 0; i < size; i++)
        {
                sum = sum + data[i];
        }

        return (sum / size);

}


double mean_vector(const vector<double> data, int start, int end)
{
	double size = 1.0*(end - start);
        double sum = 0;
        for(int i = start; i < end; i++)
        {
                sum = sum + data[i];
        }

        return (sum / size);

}

double standard_deviation_vector(const vector<double> data)
{
	int n = data.size();
	double mean=0.0, sum_deviation=0.0;
	for(int i=0; i<n;++i)
	{
		mean+=data[i];
	}
	mean=mean/n;
	for(int i=0; i<n;++i)
		sum_deviation+=(data[i]-mean)*(data[i]-mean);
	return sqrt(sum_deviation/n);
}

double standard_deviation_vector(const vector<double> data, int start, int end)
{
	int n = (end - start);
	double mean=0.0, sum_deviation=0.0;
	for(int i=start; i<end;++i)
	{
		mean+=data[i];
	}
	mean=mean/n;
	for(int i=start; i<end;++i)
		sum_deviation+=(data[i]-mean)*(data[i]-mean);
	return sqrt(sum_deviation/n);
}

