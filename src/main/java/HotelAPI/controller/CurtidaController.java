package HotelAPI.controller;


import HotelAPI.model.Curtida;
import HotelAPI.repository.CurtidaRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("curtida")
public class CurtidaController {

    private CurtidaRepository curtidaRepository;

    @Autowired
    public void setCurtidaRepository(CurtidaRepository curtidaRepository) {
        this.curtidaRepository = curtidaRepository;
    }

    //Get em todas
    @GetMapping
    public List<Curtida> findAll() {
        return curtidaRepository.findAll();
    }

    //Get por id
    @GetMapping("{id}")
    public Curtida findById(@PathVariable int id) {
        return curtidaRepository.findById(id).orElse(null);
    }


    //Post
    @PostMapping
    public Curtida save(@RequestBody Curtida curtida) {
        return curtidaRepository.save(curtida);
    }

    //Delete
    @DeleteMapping("delete{id}")
    public void deleteById(@PathVariable int id) {
        curtidaRepository.deleteById(id);
    }



}

