package HotelAPI.repository;

import HotelAPI.model.Stories;
import org.springframework.data.jpa.repository.JpaRepository;

public interface StoriesRepository extends JpaRepository<Stories, Integer> {
}
